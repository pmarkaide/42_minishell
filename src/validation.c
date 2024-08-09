/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/09 16:00:42 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(char *infile)
{
	// if (macro->here_doc == 1)
	// 	here_doc(macro);
	int fd;
	fd = open(infile, O_RDONLY);
	// if (errno == ENOENT)
	// 	free_data_and_exit(macro, macro->infile, NO_FILE);
	// if (errno == EACCES)
	// 	free_data_and_exit(macro, macro->infile, PERMISSION_DENIED);
	// if (errno == IS_DIRECTORY)
	// 	free_data_and_exit(macro, macro->infile, IS_DIRECTORY);
	return(fd);
}

int	open_outfile(char *outfile)
{
	// if (macro->here_doc == 1)
	// 	macro->out_fd = open(macro->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// else
	int fd;
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// if (errno == ENOENT)
	// 	free_data_and_exit(macro, macro->outfile, NO_FILE);
	// if (errno == EACCES)
	// 	free_data_and_exit(macro, macro->outfile, PERMISSION_DENIED);
	// if (errno == IS_DIRECTORY)
	// 	free_data_and_exit(macro, macro->infile, IS_DIRECTORY);
	return(fd);
}

int open_last_infile(t_token *redir)
{
    t_token *tmp;
    int fd;

    tmp = redir;
    fd = -1;

    while (tmp)
    {
        if (tmp->type == INRED)
        {
            fd = open_infile(tmp->value);
            if (fd == -1)
                break;
            if (!is_last_of_type(tmp, INRED))
            {
                close(fd);
                fd = -1;
            }
        }
        tmp = tmp->next;
    }
    return fd;
}

int open_last_outfile(t_token *redir)
{
    t_token *tmp;
    int fd;

    tmp = redir;
    fd = -1;

    while (tmp)
    {
        if (tmp->type == OUTRED)
        {
            fd = open_outfile(tmp->value);
            if (fd == -1)
                break;
            if (!is_last_of_type(tmp, OUTRED))
            {
                close(fd);
                fd = -1;
            }
        }
        tmp = tmp->next;
    }
    return fd;
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

// void	dup2_or_exit(t_macro *macro, int oldfd, int newfd)
// {
// 	if (dup2(oldfd, newfd) < 0)
// 		free_data_and_exit(macro, "dup2 error", -1);
// }

void dup_file_descriptors(t_macro *macro, t_cmd *cmd)
{
    int fd;

    fd = open_last_outfile(cmd->redir);
    if (fd >= 0)
        dup2(fd, STDOUT_FILENO);
    else
        dup2(macro->pipe_fd[1], STDOUT_FILENO);

    fd = open_last_infile(cmd->redir);
    if (fd >= 0)
        dup2(fd, STDIN_FILENO);
    else
        dup2(macro->pipe_fd[0], STDIN_FILENO);
}

