/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:40 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/03 23:20:54 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_numeric_argument(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (ft_isdigit(arg[i]) == 0 && arg[0] != '+' && arg[0] != '-')
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			return (2);
		}
		i++;
	}
	return (0);
}

int	ft_exit2(char **args, t_macro *macro)
{
	int	argc;
	int	code;
	int	validation_result;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (argc == 1)
	{
		macro->exit_flag = 69;
		return (0);
	}
	validation_result = validate_numeric_argument(args[1]);
	if (validation_result != 0)
		return (validation_result);
	macro->exit_flag = 69;
	code = ft_atoi(args[1]);
	if (code > 255 || code < 0)
		code = code % 256;
	return (code);
}
