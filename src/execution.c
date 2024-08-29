/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 21:51:31 by pmarkaid         ###   ########.fr       */
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
		return(-1);
	if(dup_file_descriptors(macro, macro->cmds, 0) == -1)
		return(-1);
	cmd_array = build_cmd_args_array(macro->cmds->cmd_arg, macro);
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
	ft_putstr_fd("pass validation\n",2);
	if(dup_file_descriptors(macro, macro->cmds, read_end) == -1)
		exit(macro->exit_code);
	ft_putstr_fd("pass dup\n",2);
	cmd_array = build_cmd_args_array(cmd->cmd_arg, macro);
	if(!cmd_array)
		exit_error(cmd->cmd_arg->value, "malloc error", macro, -1);
	ft_putstr_fd("pass cmd_array\n",2);
	if (cmd->type == BUILTIN)
		execute_builtin(macro, cmd_array);
	else
	{	ft_putstr_fd("going to execve\n",2);
		execve(cmd_array[0], cmd_array, macro->env);
	}
	status = macro->exit_code;
	if (index == macro->num_cmds - 1)
		write_pipe_exit(macro->pipe_exit, status);
	exit(status);
}

static int	execute_cmds(t_macro *macro, int read_end)
{
	int	i;
	int status;

	i = 0;
	while (i < macro->num_cmds)
	{
		if (pipe(macro->pipe_fd) == -1)
			return (error_msg(NULL, "pipe failed", i));
		macro->pid[i] = fork();
		if (macro->pid[i] < 0)
		{
			close_fds(macro, read_end);
			return (error_msg(NULL, "fork failed", i));
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
        close(macro->pipe_exit[1]); // Close the write end of the pipe in the parent
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

	if (macro->num_cmds == 1 && macro->cmds->type == BUILTIN)
		macro->exit_code = execute_single_builtin(macro);
	else
	{
		//TODO: add pipe and pid malloc in helper function
		if (pipe(macro->pipe_exit) == -1)
		{
			error_msg(macro, "pipe", -1);
			return ;
		}
        printf("execution :: created pipe_exit[%d, %d]\n", macro->pipe_exit[0], macro->pipe_exit[1]);
		read_end = 0;
		macro->pid = malloc(sizeof(pid_t) * macro->num_cmds);
		if(macro->pid == NULL)
		{
			error_msg(macro, "malloc", -1);
			return ;
		}
		num_cmds_executed = execute_cmds(macro, read_end);
		i = 0;
		ft_putstr_fd("going to wait\n",2);
		while (i < num_cmds_executed)
			status = wait_processes(macro->pid[i++]);
		read_pipe_exit(macro->pipe_exit, &status);
		macro->exit_code = status;
		ft_free((void**)&macro->pid);
		close_fds(macro, read_end);
	}
	return ;
}
