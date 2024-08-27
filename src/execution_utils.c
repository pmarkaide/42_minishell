/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/27 10:48:17 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(int *pipe_fd, int read_end)
{
	if (pipe_fd[0] != -1)
	{
		close(pipe_fd[0]);
		pipe_fd[0] = -1;
	}
	if (pipe_fd[1] != -1)
	{
		close(pipe_fd[1]);
		pipe_fd[1] = -1;
	}
	if (read_end > 0)
		close(read_end);
}

void	catch_parent_exit(int *pipe_exit, int *g_exit)
{
	close(pipe_exit[1]);
	read(pipe_exit[0], g_exit, sizeof(int));
	close(pipe_exit[0]);
}

int	wait_processes(pid_t pid)
{
	int	status;
	int	exit_code;

	exit_code = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	return (exit_code);
}

char	**build_cmd_args_array(t_token *cmd_args)
{
	char	**cmd_array;
	t_token	*tmp;
	int		i;

	tmp = cmd_args;
	if (!cmd_args)
		return (NULL);
	cmd_array = (char **)malloc(sizeof(char *) * (tokens_size(cmd_args) + 1));
	if (!cmd_array)
		return (NULL);
	i = 0;
	tmp = cmd_args;
	while (tmp)
	{
		cmd_array[i] = ft_strdup(tmp->value);
		if (!cmd_array[i])
		{
			free_array(&cmd_array);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}

char	**prepare_child_execution(t_macro *macro, t_cmd *cmd)
{
	char	**cmd_array;

	if (cmd->type == CMD)
		validate_executable(macro, cmd);
	cmd_array = build_cmd_args_array(cmd->cmd_arg);
	if (!cmd_array)
		exit(errno);
	return (cmd_array);
}
