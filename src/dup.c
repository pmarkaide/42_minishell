/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 23:24:13 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/02 09:19:38 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*get_last_redir(t_token *redir, char *redir_type)
{
	t_token	*tmp;
	t_token	*last_redir;

	tmp = redir;
	last_redir = NULL;
	while (tmp)
	{
		if (is_redir(tmp, redir_type))
			last_redir = tmp;
		tmp = tmp->next;
	}
	return (last_redir);
}

static int	open_last_redir(t_token *redir, char *redir_type, t_macro *macro)
{
	t_token	*last_redir;
	int		fd;

	last_redir = get_last_redir(redir, redir_type);
	fd = -1;
	if (last_redir)
	{
		if (last_redir->type == HERE_DOC)
			fd = ft_atoi(last_redir->value);
		else
			fd = open_file(last_redir, macro);
	}
	return (fd);
}

static int	dup_stdout(t_macro *macro, t_cmd *cmd)
{
	int	fd;

	fd = open_last_redir(cmd->redir, "output", macro);
	if (fd >= 1)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			return (-1);
		}
		close(fd);
		return (0);
	}
	else if (cmd->n < macro->num_cmds)
	{
		if (dup2(macro->pipe_fd[1], STDOUT_FILENO) < 0)
			return (-1);
	}
	if (macro->pipe_fd[1] != -1)
		close(macro->pipe_fd[1]);
	return (0);
}

static int	dup_stdin(t_macro *macro, t_cmd *cmd, int read_end)
{
	int	fd;

	fd = open_last_redir(cmd->redir, "input", macro);
	if (fd >= 1)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			return (-1);
		}
		close(fd);
	}
	else if (cmd->n > 1)
	{
		if (dup2(read_end, STDIN_FILENO) < 0)
			return (-1);
	}
	if (macro->pipe_fd[0] != -1)
		close(macro->pipe_fd[0]);
	return (0);
}

int	dup_file_descriptors(t_macro *macro, t_cmd *cmd, int read_end)
{
	if (dup_stdout(macro, cmd) == -1)
	{
		macro->exit_code = errno;
		perror("dup_stdout");
		return (-1);
	}
	if (dup_stdin(macro, cmd, read_end) == -1)
	{
		macro->exit_code = errno;
		perror("dup_stdin");
		return (-1);
	}
	return (0);
}
