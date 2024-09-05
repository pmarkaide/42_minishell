/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/05 23:29:43 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit;

static char	*read_line(t_macro *macro)
{
	char	*line;
	char	*path;

	path = create_path(macro);
	line = readline(path);
	free(path);
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

static void	run_shell(t_macro *macro)
{
	char	*line;
	int		status;

	while (1)
	{
		macro->exit_flag = 0;
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
		if (macro->exit_flag == 69)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_macro	*macro;
	int		status;

	(void)argc;
	macro = init_macro(envp, argv);
	signal(SIGINT, sigint_handler_in_parent);
	signal(SIGQUIT, SIG_IGN);
	run_shell(macro);
	status = macro->exit_code;
	free_macro(macro);
	exit(status);
}
