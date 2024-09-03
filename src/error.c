/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:00:20 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/03 23:19:37 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(t_macro *macro, char *file, int exit_code)
{
	if (macro)
		macro->exit_code = exit_code;
	ft_putstr_fd("minishell: ", 2);
	perror(file);
	return (exit_code);
}

void	exit_error(char *file, char *msg, t_macro *macro, int exit_code)
{
	int	status;

	status = exit_code;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	free_macro(macro);
	exit(status);
}

void	exit_free(t_macro *macro)
{
	int	status;

	status = macro->exit_code;
	free_macro(macro);
	exit(status);
}
