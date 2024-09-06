/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 21:38:08 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/06 23:56:15 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_invalid_identifier(char *arg, int *exit_flag, int name_flag)
{
	if (name_flag == 1)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		*exit_flag = 1;
	}
	else if (name_flag == 2)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': invalid option", STDERR_FILENO);
		*exit_flag = 2;
	}
}

void	print_export_var(char *var, char *value)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(var, STDOUT_FILENO);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putstr_fd(value, STDOUT_FILENO);
	ft_putstr_fd("\"\n", STDOUT_FILENO);
}

int	validate_and_clean_argument(char *arg, int *exit_flag)
{
	int	name_flag;
	
	name_flag = check_export(arg);
	if (name_flag != 0)
	{
		handle_invalid_identifier(arg, exit_flag, name_flag);
		return (-1);
	}
	return (0);
}

void	export_argless(t_macro *macro)
{
	int		i;
	char	*var;
	char	*value;

	i = 0;
	while (macro->env[i])
	{
		if (ft_strchr(macro->env[i], '='))
		{
			var = ft_substr(macro->env[i], 0, ft_strchr_i(macro->env[i], '='));
			value = ft_substr(macro->env[i], ft_strchr_i(macro->env[i], '=')
					+ 1, ft_strlen(macro->env[i]));
			if (!value)
				ft_putchar_fd('\n', STDOUT_FILENO);
			else
				print_export_var(var, value);
			free_2_strings(&var, &value);
		}
		else
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(macro->env[i], STDOUT_FILENO);
		}
		i++;
	}
}

int	check_export(char *arg)
{
	int	i;
	int	len;

	i = 0;
	if (arg[i] == '-')
		return (2);
	if (arg[i] != '_' && !ft_isalpha(arg[i]))
		return (1);
	len = ft_strchr_i(arg, '=');
	while (arg[i] && arg[i] != '=' && (i < len || len == -1))
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (1);
		i++;
	}
	return (0);
}
