/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 09:11:22 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/08 01:31:32 by dbejar-s         ###   ########.fr       */
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

	cmd = ft_strjoin(name, "=", 0);
	cmd = ft_strjoin(cmd, value, 0);
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
