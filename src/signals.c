/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:18:46 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/27 10:48:57 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

void	ft_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		g_exit = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
