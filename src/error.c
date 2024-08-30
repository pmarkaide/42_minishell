/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:00:20 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 16:59:53 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(t_macro *macro, char *file, int exit_code)
{
	if(macro)
		macro->exit_code = exit_code;
	ft_putstr_fd("minishell: ", 2);
	perror(file);
	return (exit_code);
}

void	exit_error(char *file, char *msg, t_macro *macro, int exit_code)
{
	macro->exit_code = exit_code;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	free_ins(macro);
	exit(exit_code);
}
