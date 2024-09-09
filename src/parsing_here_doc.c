/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_here_doc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/09/09 12:52:39 by pmarkaid         ###   ########.fr       */
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

int	process_lines(int pipe_fd[2], char **del, t_token *token, t_macro *macro)
{
	char	*line;
	char	*exp;

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
			exp = get_expanded_ins(line, macro);
		write(pipe_fd[1], exp, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free_string(&exp);
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

static int	process_cmd(t_cmd *cmd, t_macro *macro)
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

int	handle_here_doc(t_cmd *cmds, t_macro *macro)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (process_cmd(cmd, macro) == -1)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}
