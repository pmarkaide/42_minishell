/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/02 15:46:44 by dbejar-s         ###   ########.fr       */
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

static int	evaluate_line(char *line, t_macro *macro)
{
	if (line == NULL || *line == EOF)
	{
		printf("exit\n");
		return (-1);
	}
	if (ft_str_empty(line))
		return (1);
	if (line[0] != '\0')
		add_history(line);
	if (syntax_error_check(macro, line))
		return (1);
	if (g_exit > 0)
	{
		macro->exit_code = g_exit;
		g_exit = 0;
	}
	macro->ins = line;
	return (0);
}

static void	execute_commands(t_macro *macro)
{
	if (tokenizer(macro) == -1)
		return ;
	if (parsing(macro) == -1)
		return ;
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
		status = evaluate_line(line, macro);
		if (status == -1)
			break ;
		else if (status == 1)
		{
			free_string(&line);
			continue ;
		}
		execute_commands(macro);
	}
	status = macro->exit_code;
	free_macro(macro);
	exit(status);
}
