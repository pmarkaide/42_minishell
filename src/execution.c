/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/15 14:02:26 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_macro *macro, char **cmd_array)
{
	char	*builtin;
	int		exit_code;

	builtin = remove_path(cmd_array[0]);
	exit_code = select_and_run_builtin(builtin, cmd_array, macro);
	if (exit_code != 0)
		ft_putstr_fd("builtin failed\n", 2);
	return (exit_code);
}

static char	**prepare_child_execution(t_macro *macro, t_cmd *cmd)
{
	char	**cmd_array;

	if (cmd->type == CMD)
		validate_executable(macro, cmd);
	cmd_array = build_cmd_args_array(cmd->cmd_arg);
	if (!cmd_array)
		exit(errno);
	return (cmd_array);
}

static void	execute_child_process(t_macro *macro, int index, int read_end)
{
	int		i;
	t_cmd	*cmd;
	char	**cmd_array;
	int		exit_code;

	cmd = macro->cmds;
	i = 0;
	while (cmd != NULL && i++ < index)
		cmd = cmd->next;
	dup_file_descriptors(macro, cmd, read_end);
	cmd_array = prepare_child_execution(macro, cmd);
	if (cmd->type == BUILTIN)
		execute_builtin(macro, cmd_array);
	else
		execve(cmd_array[0], cmd_array, macro->env);
	exit_code = errno;
	if (exit_code != 0)
		ft_putstr_fd("execution failed\n", 2);
	exit(exit_code);
}

static int	execute_cmds(t_macro *macro, int read_end)
{
	int	i;

	i = 0;
	while (i < macro->num_cmds)
	{
		if (pipe(macro->pipe_fd) == -1)
			return (error_msg("pipe failed", i));
		macro->pid[i] = fork();
		if (macro->pid[i] == -1)
		{
			close(macro->pipe_fd[1]);
			return (error_msg("fork failed", i));
		}
		if (macro->pid[i] == 0)
			execute_child_process(macro, i, read_end);
		if (read_end > 0)
			close(read_end);
		read_end = macro->pipe_fd[0];
		close(macro->pipe_fd[1]);
		i++;
	}
	if (read_end > 0)
		close(read_end);
	return (i);
}

int	execution(t_macro *macro)
{
	int		exit_code;
	int		read_end;
	int		num_cmds_executed;
	char	**cmd_array;

	if (macro->num_cmds == 1 && macro->cmds->type == BUILTIN)
	{
		cmd_array = build_cmd_args_array(macro->cmds->cmd_arg);
		if (cmd_array == NULL)
			return (-1);
		exit_code = execute_builtin(macro, cmd_array); // wait and pid ???
		free_array(&cmd_array);
	}
	else
	{
		read_end = 0;
		macro->pid = malloc(sizeof(pid_t) * macro->num_cmds);
		num_cmds_executed = execute_cmds(macro, read_end);
		exit_code = wait_processes(macro->pid, num_cmds_executed);
		free(macro->pid);
	}
	return (exit_code);
}
