/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 23:24:13 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/14 12:31:38 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_file(t_token *token)
{
	int	fd;

	// if (macro->here_doc == 1)
	// macro->out_fd = open(macro->outfile,O_WRONLY | O_CREAT | O_APPEND,0644);
	// else
	if (token->type == INRED)
		fd = open(token->value, O_RDONLY);
	else if (token->type == OUTRED)
		fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (token->type == APPEND)
		fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (errno == ENOENT)
		return (-1);
	if (errno == EACCES)
		return (-1);
	if (errno == IS_DIRECTORY)
		return (-1);
	return (fd);
}

static int	open_last_redir_file(t_token *redir)
{
	t_token	*tmp;
	int		fd;

	tmp = redir;
	fd = -1;
	while (tmp)
	{
		if (tmp->type == HERE_DOC)
			fd = ft_atoi(tmp->value);
		else
		{
			fd = open_file(tmp);
			if (fd == -1)
				break ;
		}
		if (tmp->next != NULL)
		{
			close(fd);
			fd = -1;
		}
		tmp = tmp->next;
	}
	return (fd);
}

static void	dup_stdout(t_macro *macro, t_cmd *cmd)
{
	int	fd;

	fd = open_last_redir_file(cmd->out_redir);
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
	close(macro->pipe_fd[1]);
}

static void	dup_stdin(t_macro *macro, t_cmd *cmd, int read_end)
{
	int	fd;

	// add here_doc fd here if the only one
	fd = open_last_redir_file(cmd->in_redir);
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
	close(macro->pipe_fd[0]);
}

void	dup_file_descriptors(t_macro *macro, t_cmd *cmd, int read_end)
{
	dup_stdout(macro, cmd);
	dup_stdin(macro, cmd, read_end);
}
