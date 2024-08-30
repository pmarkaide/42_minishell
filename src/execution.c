/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/30 00:07:37 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(t_macro *macro, char **cmd_array)
{
	char	*builtin;

	builtin = remove_path(cmd_array[0]);
	macro->exit_code = select_and_run_builtin(builtin, cmd_array, macro);
	free_array(&cmd_array);
}

int restore_fds(int saved_stdout, int saved_stdin) {
    
	int result;
	
	result = 0;
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2 :: restore_fds");
        result = -1;
    }
    if (dup2(saved_stdin, STDIN_FILENO) == -1) {
        perror("dup2 :: restore_fds");
        result = -1;
    }
    close(saved_stdout);
    close(saved_stdin);
    return (result);
}

int	execute_single_builtin(t_macro *macro)
{
	char	**cmd_array;
	int		saved_stdout;
	int		saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if(saved_stdin == -1 || saved_stdout == -1)
	{
		error_msg(macro, "dup", -1);
		return(-1);
	}
	if(validate_redirections(macro->cmds->redir, macro) == -1)
		return(1);
	if(dup_file_descriptors(macro, macro->cmds, 0) == -1)
		return(-1);
	cmd_array = build_cmd_args_array(macro->cmds->cmd_arg);
	if(cmd_array == NULL)
	{
		error_msg(macro, "malloc", -1);
		restore_fds(saved_stdout, saved_stdin);
	}
	else
		execute_builtin(macro, cmd_array);
	restore_fds(saved_stdout, saved_stdin);
	return(0);
}

static void	execute_child_process(t_macro *macro, int index, int read_end)
{
	int		i;
	t_cmd	*cmd;
	char	**cmd_array;
	int		status;

	close(macro->pipe_exit[0]);
	cmd = macro->cmds;
	i = 0;
	while (cmd != NULL && i++ < index)
		cmd = cmd->next;
	validation(macro, cmd);
	if(dup_file_descriptors(macro, cmd, read_end) == -1)
		exit(macro->exit_code);
	cmd_array = build_cmd_args_array(cmd->cmd_arg);
	if(!cmd_array)
		exit(macro->exit_code);
	if (cmd->type == BUILTIN)
		execute_builtin(macro, cmd_array);
	else
		execve(cmd_array[0], cmd_array, macro->env);
	status = macro->exit_code;
	if (index == macro->num_cmds - 1)  // TODO: split into function
		write_pipe_exit(macro->pipe_exit, status);
	exit(status);
}

static int	execute_cmds(t_macro *macro, int read_end)
{
	int	i;
	int	status;

	i = 0;
	while (i < macro->num_cmds)
	{
		if (pipe(macro->pipe_fd) == -1)
			return (error_msg(macro, "pipe failed", i));
		macro->pid[i] = fork();
		if (macro->pid[i] < 0)
		{
			close_fds(macro, read_end);
			return (error_msg(macro, "fork failed", i));
		}
		else if (macro->pid[i] == 0)
			execute_child_process(macro, i, read_end);
		if (read_end > 0)
			close(read_end);
		read_end = macro->pipe_fd[0];
		close(macro->pipe_fd[1]);
		i++;
	}
	if (macro->pid != 0)
	{
		read_pipe_exit(macro->pipe_exit, &status);
		macro->exit_code = status;
	}
	return (i);
}

void	execution(t_macro *macro)
{
	int		read_end;
	int		num_cmds_executed;
	int		i;
	int		status;

	if (macro->cmds == NULL)
		return ;
	if (macro->num_cmds == 1 && macro->cmds->type == BUILTIN)
		macro->exit_code = execute_single_builtin(macro);
	else
	{
		if (pipe(macro->pipe_exit) == -1)
		{
			error_msg(macro, "pipe failed", 0);
			return ;
		}
		read_end = 0;
		macro->pid = malloc(sizeof(pid_t) * macro->num_cmds);
		if(macro->pid == NULL)
		{
			error_msg(macro, "malloc failed", 0);
			return ;
		}
		num_cmds_executed = execute_cmds(macro, read_end);
		i = 0;
		while (i < num_cmds_executed)
			status = wait_processes(macro->pid[i++]);
		if (macro->pid != 0) //is this correct and need? you are in the parent
			read_pipe_exit(macro->pipe_exit, &status);
		macro->exit_code = status;
		free(macro->pid);
		macro->pid = NULL;
		close_fds(macro, read_end);
	}
	return ;
}
