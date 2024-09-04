/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/04 10:20:16 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_macro *macro)
{
	if (macro->pipe_fd[0] != -1)
	{
		close(macro->pipe_fd[0]);
		macro->pipe_fd[0] = -1;
	}
	if (macro->pipe_fd[1] != -1)
	{
		close(macro->pipe_fd[1]);
		macro->pipe_fd[1] = -1;
	}
	if (macro->read_end > 0)
		close(macro->read_end);
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
	int		i;

	if (!cmd_args)
		return (NULL);
	cmd_array = (char **)malloc(sizeof(char *) * (tokens_size(cmd_args) + 1));
	if (!cmd_array)
		return (NULL);
	i = 0;
	while (cmd_args)
	{
		cmd_array[i] = ft_strdup(cmd_args->value);
		if (!cmd_array[i])
		{
			free_array(&cmd_array);
			return (NULL);
		}
		i++;
		cmd_args = cmd_args->next;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}
