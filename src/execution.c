/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/07 15:46:48 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child_process(t_macro *macro, int index)
{
	int		i;
	t_cmd	*cmd;
	char	**cmd_array;
	int		status;

	cmd = macro->cmds;
	i = 0;
	while (cmd != NULL && i++ < index)
		cmd = cmd->next;
	validation(macro, cmd);
	if (dup_file_descriptors(macro, cmd) == -1)
		exit_free(macro);
	cmd_array = build_cmd_args_array(cmd->cmd_arg);
	if (!cmd_array)
		exit_free(macro);
	if (cmd->type == BUILTIN)
		execute_builtin(macro, cmd_array);
	else
		execve(cmd_array[0], cmd_array, macro->env);
	status = macro->exit_code;
	free_macro(macro);
	exit(status);
}

static int	execute_cmds(t_macro *macro)
{
	int	i;

	i = 0;
	while (i < macro->num_cmds)
	{
		if (pipe(macro->pipe_fd) == -1)
			return (error_msg(macro, "pipe failed", i));
		signal(SIGINT, sigint_handler_in_child);
		signal(SIGQUIT, sigquit_handler_in_child);
		macro->pid[i] = fork();
		if (macro->pid[i] < 0)
		{
			close_fds(macro);
			return (error_msg(macro, "fork failed", i));
		}
		else if (macro->pid[i] == 0)
			execute_child_process(macro, i);
		if (macro->read_end > 0)
			close(macro->read_end);
		macro->read_end = macro->pipe_fd[0];
		close_fd(macro->pipe_fd[1]);
		i++;
	}
	return (i);
}

int	prepare_execution(t_macro *macro)
{
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
	int	num_cmds_executed;
	int	i;
	int	status;

	macro->read_end = -1;
	if (macro->cmds == NULL)
		return ;
	if (macro->num_cmds == 1 && macro->cmds->type == BUILTIN)
		macro->exit_code = execute_single_builtin(macro);
	else
	{
		if (prepare_execution(macro) == -1)
			return ;
		num_cmds_executed = execute_cmds(macro);
		i = 0;
		while (i < num_cmds_executed)
			status = wait_processes(macro->pid[i++]);
		macro->exit_code = status;
		ft_free((void **)&macro->pid);
		close_fds(macro);
		signal(SIGINT, sigint_handler_in_parent);
		signal(SIGQUIT, SIG_IGN);
	}
	return ;
}
