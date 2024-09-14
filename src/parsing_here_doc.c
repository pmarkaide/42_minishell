/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_here_doc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:11:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/14 20:12:44 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

static void	open_stin(t_macro *macro, int pipe_fd[2], char **del, char *line)
{
	int	fd;

	fd = open("/dev/tty", O_RDONLY);
	if (fd == -1)
	{
		macro->exit_code = 1;
		perror("open /dev/tty failed");
	}
	g_exit = 0;
	macro->exit_code = 130;
	macro->here_doc_flag = 1;
	close_fd(&pipe_fd[0]);
	close_fd(&pipe_fd[1]);
	free_2_strings(&line, del);
}

static int	process_lines(int pipe_fd[2], char **del, t_token *token,
		t_macro *macro)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_exit == SIGINT)
		{
			open_stin(macro, pipe_fd, del, line);
			return (-1);
		}
		if (!line || ft_strcmp(line, *del) == 0)
		{
			free_2_strings(&line, del);
			break ;
		}
		if (!ft_isquote(token->value[0]))
			line = get_expanded_doc(line, macro);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free_string(&line);
	}
	return (0);
}

static int	read_here_doc(t_token *token, t_macro *macro)
{
	char	*del;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (error_msg(macro, "pipe error\n", -1));
	del = clean_quotes(token->value);
	signal(SIGINT, sigint_handler_here_doc);
	if (process_lines(pipe_fd, &del, token, macro) == -1)
	{
		signal(SIGINT, sigint_handler_in_parent);
		return (-1);
	}
	close_fd(&pipe_fd[1]);
	signal(SIGINT, sigint_handler_in_parent);
	return (pipe_fd[0]);
}

int	process_cmd(t_cmd *cmd, t_macro *macro)
{
	t_token	*token;
	int		fd;

	if (cmd->redir)
	{
		token = cmd->redir;
		while (token)
		{
			if (token->type == HERE_DOC)
			{
				fd = read_here_doc(token, macro);
				if (fd == -1)
					return (-1);
				free_string(&token->value);
				token->value = ft_itoa(fd);
			}
			token = token->next;
		}
	}
	return (0);
}
