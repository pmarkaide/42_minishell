/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 08:50:33 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/15 20:56:38 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_matrixlen(char **matrix)
{
	int	i;

	i = 0;
	while (matrix && matrix[i])
		i++;
	return (i);
}

char	**ft_add_row(char **input, char *to_add)
{
	char	**result;
	int		len;
	int		i;

	i = -1;
	result = NULL;
	if (!to_add)
		return (input);
	len = ft_matrixlen(input);
	result = malloc(sizeof(char *) * (len + 2));
	result[len + 1] = NULL;
	if (!result)
		return (input);
	while (++i < len)
	{
		result[i] = ft_strdup(input[i]);
		if (!result[i])
		{
			ft_free_matrix(&input);
			ft_free_matrix(&result);
		}
	}
	result[i] = ft_strdup(to_add);
	ft_free_matrix(&input);
	return (result);
}

char	**copy_env(char **envp)
{
	char	**result;
	int		n_rows;
	int		i;

	i = 0;
	n_rows = ft_matrixlen(envp);
	result = malloc(sizeof(char *) * (n_rows + 1));
	if (!result)
		return (NULL);
	while (envp[i])
	{
		result[i] = ft_strdup(envp[i]);
		if (!result[i])
		{
			ft_free_matrix(&result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}
