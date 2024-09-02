/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/02 09:14:59 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (dup_file_descriptors(macro, cmd, read_end) == -1)
		exit(macro->exit_code);
	cmd_array = build_cmd_args_array(cmd->cmd_arg);
	if (!cmd_array)
		exit(macro->exit_code);
	if (cmd->type == BUILTIN)
		execute_builtin(macro, cmd_array);
	else
		execve(cmd_array[0], cmd_array, macro->env);
	status = macro->exit_code;
	if (index == macro->num_cmds - 1)
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
		read_pipe_exit(macro->pipe_exit, &status);
	return (i);
}

int	prepare_execution(t_macro *macro, int *read_end)
{
	if (pipe(macro->pipe_exit) == -1)
	{
		error_msg(macro, "pipe failed", 0);
		return (-1);
	}
	*read_end = 0;
	macro->pid = malloc(sizeof(pid_t) * macro->num_cmds);
	if (macro->pid == NULL)
	{
		error_msg(macro, "malloc failed", 0);
		return (-1);
	}
	return (0);
}

void	execution(t_macro *macro)
{
	int	read_end;
	int	num_cmds_executed;
	int	i;
	int	status;

	if (macro->cmds == NULL)
		return ;
	if (macro->num_cmds == 1 && macro->cmds->type == BUILTIN)
		macro->exit_code = execute_single_builtin(macro);
	else
	{
		if (prepare_execution(macro, &read_end) == -1)
			return ;
		num_cmds_executed = execute_cmds(macro, read_end);
		i = 0;
		while (i < num_cmds_executed)
			status = wait_processes(macro->pid[i++]);
		read_pipe_exit(macro->pipe_exit, &status);
		macro->exit_code = status;
		ft_free((void **)&macro->pid);
		close_fds(macro, read_end);
	}
	return ;
}
