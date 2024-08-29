/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:49 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/29 14:26:45 by dbejar-s         ###   ########.fr       */
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

int	ft_unset2(char **args, t_macro *macro)
{
	int	i;
	int	j;
	int	len;
	int	len_env;

	i = 1;
	while (args[i])
	{
		j = 0;
		len = ft_strlen(args[i]);
		while (macro->env[j])
		{
			len_env = ft_strchr_i(macro->env[j], '=');
			if ((ft_strncmp(args[i], macro->env[j], len_env) == 0
					&& len == len_env) || (len_env == -1
					&& ft_strncmp(args[i], macro->env[j], len) == 0))
			{
				remove_env_var(macro->env, j);
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}
