/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:18:46 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/06 09:07:15 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

void	sigint_handler_in_parent(int sig)
{
	if (sig == SIGINT)
    {
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit = sig;
	}
}

void	sigint_handler_in_child(int sig)
{
	if (sig == SIGINT)
		ft_printf("\n");
}

void	sigquit_handler_in_child(int sig)
{
	if (sig == SIGQUIT)
    {
		ft_printf("\n");
		ft_printf("Quit\n");
	}
}

void	sigint_handler_after_here_doc(int sig)
{
	if (sig == SIGINT)
    {
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit = sig;
	}
}

void	sigint_handler_here_doc(int sig)
{
	if (sig == SIGINT)
	g_exit = SIGINT;
	close(STDIN_FILENO);
}