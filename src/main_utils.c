/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 08:50:33 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/03 17:32:48 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_replace_matrix_row(char ***whole, char **minus, int n)
{
	char	**aux;
	int		i[3];

	i[0] = -1;
	i[1] = -1;
	i[2] = -1;
	if (!whole || !*whole || n < 0 || n >= ft_matrixlen(*whole))
		return (NULL);
	aux = ft_calloc(ft_matrixlen(*whole) + ft_matrixlen(minus), sizeof(char *));
	while (aux && whole[0][++i[0]])
	{
		if (i[0] != n)
			aux[++i[2]] = ft_strdup(whole[0][i[0]]);
		else
		{
			while (minus && minus[++i[1]])
				aux[++i[2]] = ft_strdup(minus[i[1]]);
		}
	}
	ft_free_matrix(whole);
	*whole = aux;
	return (*whole);
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

void	ft_free_matrix(char ***matrix)
{
	int	i;

	if (matrix && *matrix)
	{
		i = 0;
		while ((*matrix)[i])
		{
			free((*matrix)[i]);
			i++;
		}
		free(*matrix);
		*matrix = NULL;
	}
}

int	ft_matrixlen(char **matrix)
{
	int	i;

	i = 0;
	while (matrix && matrix[i])
		i++;
	return (i);
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
