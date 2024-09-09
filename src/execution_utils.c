/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/09 12:05:54 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_fd(int *fd)
{
    if (*fd != -1)
    {
        close(*fd);
        *fd = -1;
    }
}

static void close_all_heredoc(t_macro *macro)
{
	t_cmd	*cmd;
	t_token	*redir;
	int		fd;

	cmd = macro->cmds;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == HERE_DOC)
			{
				fd = ft_atoi(redir->value);
				close_fd(&fd);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}


void	close_fds(t_macro *macro)
{
	close_fd(&macro->pipe_fd[0]);
	close_fd(&macro->pipe_fd[1]);
	if (macro->read_end > 0)
		close(macro->read_end);
	close_all_heredoc(macro);
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
