/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 02:03:09 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/08 01:39:59 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_state;

/*LAS SEÑALES NO ESTAN LISTAS, PARA PROBAR PUEDES USAR CTRL+C PARA SALIR DEL SHELL
void	ft_signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        g_state = 130;
        printf("capturo Ctrl+C\n");
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}*/