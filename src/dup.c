/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 23:24:13 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/26 14:54:18 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

static int	open_last_redir_file(t_token *redir, char *redir_type)
{
	t_token	*tmp;
	t_token	*last_redir;
	int		fd;

	last_redir = NULL;
	tmp = redir;
	while (tmp)
	{
		if (is_redir(tmp, redir_type))
			last_redir = tmp;
		tmp = tmp->next;
	}
	fd = -1;
	tmp = redir;
	while (tmp)
	{
		if (tmp == last_redir)
		{
			if (last_redir->type == HERE_DOC)
				fd = ft_atoi(last_redir->value);
			else
				fd = open_file(last_redir);
		}
		tmp = tmp->next;
	}
	return (fd);
}

static void	dup_stdout(t_macro *macro, t_cmd *cmd)
{
	int	fd;

	fd = open_last_redir_file(cmd->redir, "output");
	if (fd >= 1)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			close(fd);
			return ;
		}
		close(fd);
	}
	else if (cmd->n < macro->num_cmds)
	{
		if (dup2(macro->pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 pipe write");
			return ;
		}
	}
	if (macro->pipe_fd[1] != -1)
		close(macro->pipe_fd[1]);
}

static void	dup_stdin(t_macro *macro, t_cmd *cmd, int read_end)
{
	int	fd;

	fd = open_last_redir_file(cmd->redir, "input");
	if (fd >= 1)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			close(fd);
			return ;
		}
		close(fd);
	}
	else if (cmd->n > 1)
	{
		if (dup2(read_end, STDIN_FILENO) < 0)
		{
			perror("dup2 pipe read");
			return ;
		}
	}
	if (macro->pipe_fd[0] != -1)
		close(macro->pipe_fd[0]);
}

void	dup_file_descriptors(t_macro *macro, t_cmd *cmd, int read_end)
{
	dup_stdout(macro, cmd);
	dup_stdin(macro, cmd, read_end);
}
