/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/28 03:52:25 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit;

t_macro	*start_env(t_macro *macro, char **argv)
{
	char	*num;
	char	*str;

	str = getcwd(NULL, 0);
	macro->env = fix_env("PWD", str, macro->env, 3);
	free(str);
	str = grab_env("SHLVL", macro->env, 5);
	if (!str || ft_atoi(str) <= 0)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(str) + 1);
	free(str);
	macro->env = fix_env("SHLVL", num, macro->env, 5);
	free(num);
	str = grab_env("PATH", macro->env, 4);
	if (!str)
		macro->env = fix_env("PATH",
				"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", macro->env, 4);
	free(str);
	str = grab_env("_", macro->env, 1);
	if (!str)
		macro->env = fix_env("_", argv[0], macro->env, 1);
	free(str);
	return (macro);
}

static char	*grab_home(t_macro *macro)
{
	char	*home;

	home = grab_env("HOME", macro->env, 4);
	if (home == NULL)
	{
		return (NULL);
	}
	return (home);
}

t_macro	*init_macro(char **envp, char **argv)
{
	t_macro	*macro;

	macro = malloc(sizeof(t_macro));
	if (!macro)
	{
		ft_putstr_fd("Error: Malloc failed creating macro structure\n", 2);
		exit(1);
	}
	ft_bzero(macro, sizeof(t_macro));
	macro->envp = envp;
	macro->env = copy_env(envp);
	macro->history = NULL;
	macro->instruction = NULL;
	macro->tokens = NULL;
	macro->cmds = NULL;
	macro->pid = 0;
	macro = start_env(macro, argv);
	macro->m_pwd = char_pwd();
	macro->m_home = grab_home(macro);
	macro->exit_code = 0;
	return (macro);
}

int	main(int argc, char **argv, char **envp)
{
	t_macro	*macro;
	char	*line;
	char	*path;

	g_exit = 0;
	macro = init_macro(envp, argv);
	(void)argc;
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		path = ft_strjoin("minishell>", " ", NULL);
		line = readline(path);
		if (line == NULL || *line == EOF)
		{
			printf("exit\n");
			break ;
		}
		if (ft_str_empty(line))
		{
			free(line);
			continue ;
		}
		if (line[0] != '\0')
			add_history(line);
		if (syntax_error_check(line))
		{
			macro->exit_code = 2;
			free(line);
			continue ;
		}
		if (g_exit != 0)
		{
			macro->exit_code = g_exit;
			g_exit = 0;
		}
		macro->instruction = line;
		tokenizer(macro);
		macro->cmds = parsing(macro);
		if (macro->cmds == NULL)
		{	
			free(line);
			continue ;
		}
		execution(macro);
	}
	exit(0);
}
