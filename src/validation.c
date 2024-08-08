/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/08 23:15:23 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_infile(t_macro *macro)
{
	// if (macro->here_doc == 1)
	// 	here_doc(macro);
	macro->in_fd = open(macro->infile, O_RDONLY);
	if (errno == ENOENT)
		free_data_and_exit(macro, macro->infile, NO_FILE);
	if (errno == EACCES)
		free_data_and_exit(macro, macro->infile, PERMISSION_DENIED);
	if (errno == IS_DIRECTORY)
		free_data_and_exit(macro, macro->infile, IS_DIRECTORY);
}

void	open_outfile(t_macro *macro)
{
	// if (macro->here_doc == 1)
	// 	macro->out_fd = open(macro->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// else
	macro->out_fd = open(macro->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (errno == ENOENT)
		free_data_and_exit(macro, macro->outfile, NO_FILE);
	if (errno == EACCES)
		free_data_and_exit(macro, macro->outfile, PERMISSION_DENIED);
	if (errno == IS_DIRECTORY)
		free_data_and_exit(macro, macro->infile, IS_DIRECTORY);
}

void	close_open_fds(t_macro *macro)
{
	if (macro->pipe_fd[0] != -1)
		close(macro->pipe_fd[0]);
	if (macro->pipe_fd[1] != -1)
		close(macro->pipe_fd[1]);
	if (macro->in_fd != -1)
		close(macro->in_fd);
	if (macro->out_fd != -1)
		close(macro->out_fd);
}

void	dup2_or_exit(t_macro *macro, int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) < 0)
		free_data_and_exit(macro, "dup2 error", -1);
}

void	dup_file_descriptors(t_macro *macro, int cmd, int read_end)
{
	if (cmd == 0)
	{
		open_infile(macro);
		dup2_or_exit(macro, macro->in_fd, STDIN_FILENO);
		close(macro->in_fd);
		dup2_or_exit(macro, macro->pipe_fd[1], STDOUT_FILENO);
		close(macro->pipe_fd[1]);
	}
	else if (cmd == macro->num_cmds - 1)
	{
		open_outfile(macro);
		dup2_or_exit(macro, read_end, STDIN_FILENO);
		close(read_end);
		dup2_or_exit(macro, macro->out_fd, STDOUT_FILENO);
		close(macro->out_fd);
	}
	else
	{
		dup2_or_exit(macro, read_end, STDIN_FILENO);
		close(read_end);
		dup2_or_exit(macro, macro->pipe_fd[1], STDOUT_FILENO);
		close(macro->pipe_fd[1]);
	}
}
