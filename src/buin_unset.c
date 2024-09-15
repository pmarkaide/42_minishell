/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:49 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/10 15:24:24 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_var(char **env, int index)
{
	free(env[index]);
	while (env[index])
	{
		env[index] = env[index + 1];
		index++;
	}
}

static void	check_invalid(char *arg, int *status)
{
	if (arg[0] == '-')
	{
		ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': invalid option", STDERR_FILENO);
		*status = 2;
	}
}

int	ft_unset(char **args, t_macro *macro)
{
	int	ijk[3];
	int	len_env;

	ijk[0] = 0;
	ijk[2] = 0;
	while (args[++ijk[0]])
	{
		if (ijk[0] == 1)
			check_invalid(args[1], &ijk[2]);
		ijk[1] = 0;
		while (macro->env[ijk[1]])
		{
			len_env = ft_strchr_i(macro->env[ijk[1]], '=');
			if (len_env == -1)
				len_env = ft_strlen(macro->env[ijk[1]]);
			if ((ft_strncmp(args[ijk[0]], macro->env[ijk[1]], len_env) == 0
					&& len_env == (int)ft_strlen(args[ijk[0]])))
			{
				remove_env_var(macro->env, ijk[1]);
				break ;
			}
			ijk[1]++;
		}
	}
	return (ijk[2]);
}
