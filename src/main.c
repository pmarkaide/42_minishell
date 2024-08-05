/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/04 12:15:03 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_macro	*init_macro(void)
{
	t_macro	*macro;

	macro = malloc(sizeof(t_macro));
	if (!macro)
	{
		ft_putstr_fd("Error: Malloc failed creating macro structure\n", 2);
		exit(1);
	}
	ft_bzero(macro, sizeof(t_macro));
	return (macro);
}

int	main(int argc, char **argv)
{
	t_macro	*macro;

	macro = init_macro();
	// signal_catcher();
	// macro.instruction = ft_readline(stdin);
	if (argc < 2)
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", 2);
		exit(1);
	}
	macro->instruction = syntax_error_check(argv[1]);
	tokenizer(macro);
	// parsing(&macro);
	// execution(&macro);
	// free_instruction(&macro);
	// ft_readline(&macro);
	// free_macro(&macro);
	// ft_printf("%s\n", macro.instruction);
	exit(0);
}
