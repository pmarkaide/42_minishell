/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:09:31 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/08 01:38:53 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_state;

pid_t	grab_pid(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Error creating child process");
		//BORRAR CUANDO SE HAGA EL FREE
		//QUEDA CURRO AQUI
		exit(1);
	}
	if (pid > 0)
		waitpid(pid, NULL, 0);
	return (pid - 1);
}