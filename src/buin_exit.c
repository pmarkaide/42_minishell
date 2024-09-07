/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:40 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/06 23:58:30 by dbejar-s         ###   ########.fr       */
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

static int	is_number(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i] != '\0')
	{
		if (ft_isdigit(arg[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

static void	print_msgs(char *arg)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}

static int	check_args(char **args, t_macro *macro, int argc)
{
	if (argc > 2 && is_number(args[1]) == 1)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (argc > 1 && args[1][0] == '\0')
	{
		print_msgs(args[1]);
		macro->exit_flag = 70;
		return (2);
	}
	if (argc > 2 && is_number(args[1]) == 0)
	{
		print_msgs(args[1]);
		macro->exit_flag = 70;
		return (2);
	}
	if (argc == 2 && is_number(args[1]) == 0)
	{
		print_msgs(args[1]);
		macro->exit_flag = 70;
		return (2);
	}
	return (0);
}

int	ft_exit2(char **args, t_macro *macro)
{
	int	argc;
	int	code;
	int	validation_result;
	int	status;

	argc = 0;
	while (args[argc])
		argc++;
	status = check_args(args, macro, argc);
	if (status != 0)
		return (status);
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
