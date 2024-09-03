/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 09:11:22 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/03 09:28:24 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strchr_i(const char *s, int c)
{
	unsigned char	c_unsigned;
	int				i;

	i = 0;
	if (!s)
		return (-1);
	c_unsigned = (unsigned char)c;
	while (s[i] != '\0')
	{
		if (s[i] == c_unsigned)
			return (i);
		i++;
	}
	if (c_unsigned == '\0')
		return (i);
	return (-1);
}

int	var_in_env(char *argv, char **env, int ij[2])
{
	int	pos;

	ij[1] = 0;
	pos = ft_strchr_i(argv, '=');
	if (pos == -1)
		return (-1);
	while (env[ij[1]])
	{
		if (!ft_strncmp(env[ij[1]], argv, pos + 1))
			return (1);
		ij[1]++;
	}
	return (0);
}

void	ft_export_do(t_macro *macro, char *name, char *value)
{
	int		ij[2];
	int		pos;
	char	*cmd;

	cmd = ft_strjoin(name, "=", 0, 0);
	cmd = ft_strjoin(cmd, value, 0, 0);
	ij[0] = 1;
	pos = var_in_env(cmd, macro->env, ij);
	if (pos == 1)
	{
		free(macro->env[ij[1]]);
		macro->env[ij[1]] = ft_strdup(cmd);
	}
	if (pos != 1)
		macro->env = ft_add_row(macro->env, cmd);
}

char	*ft_getenv(char *var, char **env)
{
	int		i;
	int		m;
	char	*value;

	i = 0;
	m = 0;
	while (!ft_strchr(var, '=') && env && env[i])
	{
		m = ft_strchr_i(env[i], '=');
		if (!ft_strncmp(env[i], var, ft_strlen(var))
			&& m == (int)ft_strlen(var))
		{
			value = ft_substr(env[i], m + 1, ft_strlen(env[i]));
			return (value);
		}
		i++;
	}
	return (NULL);
}
