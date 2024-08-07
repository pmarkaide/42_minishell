/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/08 01:47:41 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_state;

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
	macro->commands = NULL;
	macro->pid = grab_pid(); //No hay gestion del pid aún implementada, solo lo guardo
	return (macro);
}

int	main(int ac, char **av, char **envp)
{
	t_macro	*macro;
	char	*line;

	macro = init_macro(envp);
	(void)ac;
	(void)av;
	g_state = 0;
	while (1)
	{
		/*signal(SIGINT, ft_signal_handler);
		signal(SIGQUIT, SIG_IGN);*/ //quito las señales para testear builtins de manera aislada
		line = readline("minishell> ");
		if (line == NULL)
			exit(0);
		if (line[0] != '\0')
			add_history(line);
		//FALTA FUNCION AQUI PARA AÑADIR HISTORIAL EN macro->history
		macro->instruction = syntax_error_check(line);
		tokenizer(macro);
		test_builtins(macro);
		free(line);
		// parsing(&macro);
		// execution(&macro);
		// free_instruction(&macro);
		// ft_readline(&macro);
		// free_macro(&macro);
		// ft_printf("%s\n", macro.instruction);
	}
	exit(g_state);
}
