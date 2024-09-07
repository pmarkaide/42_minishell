/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:42:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/07 14:06:03 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

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
			close_fd(fd);
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
		return (error_msg(macro, "pipe error\n", -1));
	del = clean_quotes(token->value);
	while (1)
	{
		signal(SIGINT, sigint_handler_here_doc);
		line = readline("> ");
		if (g_exit == SIGINT)
		{
			open("/dev/tty", O_RDONLY);
			g_exit = 0;
			macro->exit_code = 130;
			macro->here_doc_flag = 1;
			close_fd(pipe_fd[1]);
			free_string(&line);
			free_string(&del);
			signal(SIGINT, sigint_handler_in_parent);
			return (-1);
		}
		if (!line || ft_strcmp(line, del) == 0)
		{
			close_fd(pipe_fd[1]);
			free_string(&line);
			break ;
		}
		if (!ft_isquote(token->value[0]))
			line = get_expanded_ins(line, macro);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free_string(&line);
	}
	free_string(&del);
	close_fd(pipe_fd[1]);
	signal(SIGINT, sigint_handler_in_parent);
	return (pipe_fd[0]);
}

int	handle_here_doc(t_cmd *cmds, t_macro *macro)
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
					free_string(&token->value);
					if (fd == -1)
					{
						return (-1);
					}
					token->value = ft_itoa(fd);
				}
				token = token->next;
			}
		}
		cmd = cmd->next;
	}
	return (0);
}
