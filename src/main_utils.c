/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 08:50:33 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/28 16:19:20 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3) 
{
    int		total_len;
	char	*res;
	
	if (!s1 || !s2 || !s3)
        return (NULL);
	total_len = 0;
	total_len += ft_strlen(s1);
	total_len += ft_strlen(s2);
	total_len += ft_strlen(s3);
    res = (char *)ft_calloc(sizeof(char), (total_len + 1)); // +1 for the null terminator
    if (!res)
        return (NULL);
    ft_strcpy(res, s1);
    ft_strcat(res, s2);
    ft_strcat(res, s3);
    return (res);
}

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

void	ft_free_matrix(char ***m)
{
	int	i;

	i = 0;
	while (m && m[0] && m[0][i])
	{
		free(m[0][i]);
		i++;
	}
	if (m)
	{
		free(m[0]);
		*m = NULL;
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
