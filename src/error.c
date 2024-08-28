/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:00:20 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/28 03:29:11 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(char *file, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	perror(file);
	return (exit_code);
}

void exit_error(char *file, char *msg, t_macro *macro, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	macro->exit_code = exit_code;
	//exit(exit_code);
}
