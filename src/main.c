/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/09 21:55:15 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_macro	*init_macro(char **envp)
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
	return (macro);
}

int	main(int argc, char **argv, char **envp)
{
	t_macro	*macro;
	char	*line;
	char	*path;

	macro = init_macro(envp);
	(void)argc;
	(void)argv;
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
		// if (line == NULL || *line == EOF)
		// {
		// 	printf("Ctrl+D exits");
		// 	break;
		// }
		// if (line[0] != '\0')
		// 	add_history(line);
		//FALTA FUNCION AQUI PARA AÑADIR HISTORIAL EN macro->history
		if (syntax_error_check(line))
		{
			free(line);
			continue ;
		}
		// syntax_error_check(line);
		macro->instruction = line;
		tokenizer(macro);
		test_builtins(macro);
		parsing(macro->tokens);
		execution(macro);
		// free_instruction(&macro);
		// ft_readline(&macro);
		// free_macro(&macro);
		// ft_printf("%s\n", macro.instruction);
	
	// signal_catcher();
	// macro.instruction = ft_readline(stdin);
	// if (argc < 2)
	// {
	// 	ft_putstr_fd("Error: Invalid number of arguments\n", 2);
	// 	exit(1);
	// }
	// macro->instruction = syntax_error_check(argv[1]);
	// tokenizer(macro);
	// macro->cmds = parsing(macro->tokens);
	// execution(&macro);
	// free_instruction(&macro);
	// ft_readline(&macro);
	// free_macro(&macro);
	// ft_printf("%s\n", macro.instruction);
	}
	exit(0);
}
