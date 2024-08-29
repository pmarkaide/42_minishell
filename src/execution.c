/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 15:14:28 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_macro *macro, char **cmd_array)
{
	char	*builtin;

	builtin = remove_path(cmd_array[0]);
	macro->exit_code = select_and_run_builtin(builtin, cmd_array, macro);
	free_array(&cmd_array);
	return (macro->exit_code);
}

int	execute_single_builtin(t_macro *macro)
{
	char	**cmd_array;
	int		saved_stdout;
	int		saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if(validate_redirections(macro->cmds->redir, macro) == -1)
		return(-1);
	dup_file_descriptors(macro, macro->cmds, 0); // TODO: protect
	cmd_array = build_cmd_args_array(macro->cmds->cmd_arg, macro); // TODO: protect
	macro->exit_code = execute_builtin(macro, cmd_array);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	return (macro->exit_code);
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
	dup_file_descriptors(macro, cmd, read_end); // TODO: protect
	cmd_array = build_cmd_args_array(cmd->cmd_arg, macro);
	if (cmd->type == BUILTIN)
		macro->exit_code = execute_builtin(macro, cmd_array);
	else
		execve(cmd_array[0], cmd_array, macro->env);
	status = macro->exit_code;
	if (index == macro->num_cmds - 1)  // TODO: split into function
	{
		close(macro->pipe_exit[0]);
		write(macro->pipe_exit[1], &status, sizeof(int));
		close(macro->pipe_exit[1]);
	}
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
			return (error_msg("pipe failed", i));
		macro->pid[i] = fork();
		if (macro->pid[i] < 0)
		{
			close_fds(macro, read_end);
			return (error_msg("fork failed", i));
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
		catch_parent_exit(macro->pipe_exit, &status);
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
		execute_single_builtin(macro);
	else
	{
		if (pipe(macro->pipe_exit) == -1)
		{
			error_msg("pipe failed", 0);
			return ;
		}
		read_end = 0;
		macro->pid = malloc(sizeof(pid_t) * macro->num_cmds); //TODO: protect
		num_cmds_executed = execute_cmds(macro, read_end);
		i = 0;
		while (i < num_cmds_executed)
			status = wait_processes(macro->pid[i++]);
		if (macro->pid != 0) //is this correct and need? you are in the parent
			catch_parent_exit(macro->pipe_exit, &status);
		macro->exit_code = status;
		free(macro->pid);
		macro->pid = NULL;
		close_fds(macro, read_end);
	}
	return ;
}
