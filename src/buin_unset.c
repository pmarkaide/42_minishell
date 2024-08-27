/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:49 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/27 11:47:53 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

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
			if (ft_strncmp(args[i], macro->env[j], len_env) == 0
				&& len == len_env)
			{
				free(macro->env[j]);
				while (macro->env[j])
				{
					macro->env[j] = macro->env[j + 1];
					j++;
				}
				break ;
			}
			else if (len_env == -1 && ft_strncmp(args[i], macro->env[j],
					len) == 0)
			{
				free(macro->env[j]);
				while (macro->env[j])
				{
					macro->env[j] = macro->env[j + 1];
					j++;
				}
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}
