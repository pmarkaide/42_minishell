/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 22:29:03 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit;

static char	*read_line(t_macro *macro)
{
	char	*line;
	char	*path;

	if (g_exit == 130)
		line = readline("");
	else
	{
		path = create_path(macro);
		line = readline(path);
		free(path);
	}
	return (line);
}

static int	process_line(char *line, t_macro *macro)
{
	if (line == NULL || *line == EOF)
	{
		printf("exit\n");
		return (0);
	}
	if (ft_str_empty(line))
	{
		free(line);
		return (1);
	}
	if (line[0] != '\0')
		add_history(line);
	if (syntax_error_check(line))
	{
		macro->exit_code = 2;
		free(line);
		return (1);
	}
	if (g_exit > 0)
	{
		macro->exit_code = g_exit;
		g_exit = 0;
	}
	macro->instruction = line;
	return (2);
}

static void	execute_commands(t_macro *macro, char *line)
{
	tokenizer(macro);
	macro->cmds = parsing(macro);
	if (macro->cmds == NULL)
	{
		free_string(&line);
		return ;
	}
	execution(macro);
	free_ins(macro);
}

int	main(int argc, char **argv, char **envp)
{
	t_macro	*macro;
	char	*line;
	int		status;

	(void)argc;
	g_exit = 0;
	macro = init_macro(envp, argv);
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = read_line(macro);
		status = process_line(line, macro);
		if (status == 0)
			break ;
		else if (status == 1)
			continue ;
		execute_commands(macro, line);
	}
	free_macro(macro);
	exit(0);
}
