/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/10 13:05:41 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(char *infile)
{
	int	fd;

	// if (macro->here_doc == 1)
	// 	here_doc(macro);
	fd = open(infile, O_RDONLY);
	if (errno == ENOENT)
		return (-1);
	if (errno == EACCES)
		return (-1);
	if (errno == IS_DIRECTORY)
		return (-1);
	return (fd);
}

int	open_outfile(char *outfile)
{
	int	fd;

	// if (macro->here_doc == 1)
	//  macro->out_fd = open(macro->outfile, O_WRONLY | O_CREAT | O_APPEND,0644);
	// else
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (errno == ENOENT)
		return (-1);
	if (errno == EACCES)
		return (-1);
	if (errno == IS_DIRECTORY)
		return (-1);
	return (fd);
}

int	open_last_infile(t_token *redir)
{
	t_token	*tmp;
	int		fd;

	tmp = redir;
	fd = -1;
	while (tmp)
	{
		if (tmp->type == INRED)
		{
			fd = open_infile(tmp->value);
			if (fd == -1)
				break ;
			if (!is_last_of_type(tmp, INRED))
			{
				if (fd != 0)
					close(fd);
				fd = 0;
			}
		}
		tmp = tmp->next;
	}
	return (fd);
}

int	open_last_outfile(t_token *redir)
{
	t_token	*tmp;
	int		fd;

	tmp = redir;
	fd = -1;
	while (tmp)
	{
		if (tmp->type == OUTRED)
		{
			fd = open_outfile(tmp->value);
			if (fd == -1)
				break ;
			if (!is_last_of_type(tmp, OUTRED))
			{
				close(fd);
				fd = -1;
			}
		}
		tmp = tmp->next;
	}
	return (fd);
}

void	dup_stdout(t_macro *macro, t_cmd *cmd)
{
	int	fd;

	fd = open_last_outfile(cmd->redir);
	if (fd == -1)
		return ;
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

void	dup_stdin(t_macro *macro, t_cmd *cmd, int read_end)
{
	int	fd;

	fd = open_last_infile(cmd->redir);
	if (fd == -1)
		return ;
	if (fd >= 0)
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
