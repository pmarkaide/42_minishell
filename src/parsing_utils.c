/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:42:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/27 09:24:02 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_here_doc_not_needed(t_token *tokens)
{
	t_token	*tmp;
	t_token	*last;
	int		fd;

	last = NULL;
	tmp = tokens;
	while (tmp && is_redir(tmp, "input"))
	{
		last = tmp;
		tmp = tmp->next;
	}
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == HERE_DOC && tmp != last)
		{
			fd = ft_atoi(tmp->value);
			close(fd);
		}
		tmp = tmp->next;
	}
}

static int	read_here_doc(t_token *token, t_macro *macro)
{
	char	*line;
	char	*del;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (error_msg("pipe error\n", -1));
	del = clean_quotes(token->value);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, del) == 0)
		{
			close(pipe_fd[1]);
			free(line);
			break ;
		}
		if (!ft_isquote(token->value[0]))
			line = get_expanded_instruction(line, macro);
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	handle_here_doc(t_cmd *cmds, t_macro *macro)
{
	t_token	*token;
	t_cmd	*cmd;
	int		fd;

	cmd = cmds;
	while (cmd)
	{
		if (cmd->redir)
		{
			token = cmd->redir;
			while (token)
			{
				if (token->type == HERE_DOC)
				{
					fd = read_here_doc(token, macro);
					free(token->value);
					token->value = ft_itoa(fd);
				}
				token = token->next;
			}
		}
		cmd = cmd->next;
	}
}
