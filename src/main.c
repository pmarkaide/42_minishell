/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/02 15:08:19 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_macro	macro;

	ft_bzero(&macro, sizeof(t_macro));
	// signal_catcher();
	// macro.instruction = ft_readline(stdin);
	syntax_error_check(macro.instruction);
	// tokenizer(&macro, macro.instruction);
	// parsing(&macro);
	// execution(&macro);
	// free_instruction(&macro);
	// ft_readline(&macro);
	// free_macro(&macro);
	exit(0);
}
