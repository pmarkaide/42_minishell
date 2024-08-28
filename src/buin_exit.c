/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:40 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/28 02:09:33 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit2(char **args)
{
	int	argc;
	int	code;
	int	i;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(0);
	}
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	i = 0;
	while (args[1][i] != '\0')
	{
		if (ft_isdigit(args[1][i]) == 0 && args[1][0] != '+'
			&& args[1][0] != '-')
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			return (2);
		}
		i++;
	}
	code = ft_atoi(args[1]);
	if (code > 255 || code < 0)
		code = code % 256;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(code);
}
