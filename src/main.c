/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/18 16:21:16 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	macro->path = char_pwd();
	macro->history = NULL;
	macro->instruction = NULL;
	macro->tokens = NULL;
	macro->cmds = NULL;
	macro->pid = 0;
	macro = start_env(macro, argv);
	return (macro);
}

int	main(int argc, char **argv, char **envp)
{
	t_macro	*macro;
	char	*line;
	char	*path;

	macro = init_macro(envp, argv);
	(void)argc;
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		path = getcwd(NULL, 0);
		if (path == NULL)
		{
			perror("Error getting current directory");
		}
		path = ft_strjoin("minishell> ", path, NULL);
		path = ft_strjoin(path, " $ ", NULL);
		line = readline(path);
		if (line == NULL || *line == EOF)
		{
			printf("Ctrl+D exits");
			break ;
		}
		if (line[0] != '\0')
			add_history(line);
		// FALTA FUNCION AQUI PARA AÑADIR HISTORIAL EN macro->history
		if (syntax_error_check(line))
		{
			free(line);
			continue ;
		}
		macro->instruction = line;
		clean(macro);
		tokenizer(macro);
		// test_builtins(macro);
		macro->cmds = parsing(macro);
		execution(macro);
		// free_instruction(&macro);
		// ft_readline(&macro);
		// free_macro(&macro);
		// ft_printf("%s\n", macro.instruction);
		free(line);
	}
	exit(0);
}
