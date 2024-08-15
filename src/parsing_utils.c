/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:42:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/14 13:25:36 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static int	read_here_doc(t_token *token)
{	
	char	*line;
	int pipe_fd[2];
	
    if (pipe(pipe_fd) == -1)
		return(error_msg("pipe error\n", -1));
	while (1)
	{
        line = readline("> ");
		if (!line || ft_strcmp(line, token->value) == 0)
        {
			close(pipe_fd[1]);
            free(line);
            break;
        }
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return(pipe_fd[0]);
}

void handle_here_doc(t_cmd *cmds)
{
	t_token *token;
	t_cmd *cmd;
	int fd;
	
	cmd = cmds;
	while(cmd)
	{
		token = cmd->in_redir;
		while(token)
		{
			if(token->type == HERE_DOC)
			{
				fd = read_here_doc(token);
				free(token->value);
				token->value = ft_itoa(fd);
			}
			token = token->next;
		}
		cmd = cmd->next;
	}
}
