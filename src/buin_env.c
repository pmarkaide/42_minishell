/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:37 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/27 09:08:05 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit; 

char	*grab_env(char *var, char **env, int n)
{
	int	i;
	int	m;

	i = 0;
	m = 0;
	while (!ft_strchr(var, '=') && env && env[i])
	{
		m = n;
		if (m < ft_strchr_i(env[i], '='))
			m = ft_strchr_i(env[i], '=');
		if (!ft_strncmp(env[i], var, m))
			return (ft_substr(env[i], m + 1, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}

char	**fix_env(char *var, char *value, char **env, int n)
{
	int		i[2];
	char	*aux[2];

	i[0] = -1;
	aux[0] = ft_strjoin(var, "=", NULL);
	aux[1] = ft_strjoin(aux[0], value, NULL);
	free(aux[0]);
	while (!ft_strchr(var, '=') && env && env[++i[0]])
	{
		i[1] = n;
		if (i[1] < ft_strchr_i(env[i[0]], '='))
			i[1] = ft_strchr_i(env[i[0]], '=');
		if (!ft_strncmp(env[i[0]], var, i[1]))
		{
			aux[0] = env[i[0]];
			env[i[0]] = aux[1];
			free(aux[0]);
			return (env);
		}
	}
	env = ft_add_row(env, aux[1]);
	free(aux[1]);
	return (env);
}

int	ft_env2(t_macro *macro)
{
	int	i;

	i = 0;
	while (macro->env[i])
	{
		if (ft_strchr(macro->env[i], '='))
			ft_putendl_fd(macro->env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}