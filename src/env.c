/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 09:11:22 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/15 20:54:07 by pmarkaid         ###   ########.fr       */
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
