/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 21:38:08 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/02 16:11:55 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_var(char *var, char *value)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(var, STDOUT_FILENO);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putstr_fd(value, STDOUT_FILENO);
	ft_putstr_fd("\"\n", STDOUT_FILENO);
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
	if (arg[i] != '_' && !ft_isalpha(arg[i]))
		return (0);
	len = ft_strchr_i(arg, '=');
	while (arg[i] && arg[i] != '=' && (i < len || len == -1))
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
