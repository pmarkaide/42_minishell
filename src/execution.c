/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/09 20:10:06 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(char *msg, int exit_code)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(msg, 2);
	return (exit_code);
}

static int	get_exit_code(int status)
{
	int	exit_code;

	exit_code = 0;
	if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	return (exit_code);
}

static int	wait_processes(pid_t *pid, int cmds)
{
	int	i;
	int	status;
	int	exit_code;

	i = 0;
	exit_code = 0;
	while (i < cmds)
	{
		waitpid(pid[i], &status, 0);
		if (i == cmds - 1)
			exit_code = get_exit_code(status);
		i++;
	}
	return (exit_code);
}

static void	execute_child_process(t_macro *macro, int index, int read_end)
{
	int i;
	t_cmd *cmd;
	char **cmd_array;

	cmd = macro->cmds;
	i = 0;
	while (cmd != NULL && i < index)
    {
        cmd = cmd->next;
        i++;
    }
	dup_file_descriptors(macro, cmd, read_end);
	//eval_executable(macro, macro->cmds[i][0]);
	cmd_array = build_cmd_args_array(cmd->cmd_arg); // handle NULL return
	if (execve(cmd_array[1], cmd_array, macro->envp) == -1)
	{
		//free_data(macro);
		exit(EXIT_FAILURE);
	}
	exit(0);
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
	int	exit_code;
	int	read_end;
	int	num_cmds_executed;

	read_end = 0;
	num_cmds_executed = execute_cmds(macro, read_end);
	exit_code = wait_processes(macro->pid, num_cmds_executed);
	close(read_end);
	close_open_fds(macro);
	return (exit_code);
}
