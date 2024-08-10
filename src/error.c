/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:00:20 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/10 13:00:41 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(char *msg, int exit_code)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(msg, 2);
	return (exit_code);
}
