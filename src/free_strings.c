/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_strings.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:22:21 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/31 14:35:01 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2_strings(char **str1, char **str2)
{
	if (str1 != NULL && *str1 != NULL)
	{
		free(*str1);
		*str1 = NULL;
	}
	if (str2 != NULL && *str2 != NULL)
	{
		free(*str2);
		*str2 = NULL;
	}
}

void	free_array(char ***array)
{
	size_t	i;

	if (*array == NULL)
		return ;
	i = 0;
	while ((*array)[i] != NULL)
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}

char	*free_string(char **str)
{
	if (str != NULL && *str != NULL)
	{
		free(*str);
		*str = NULL;
	}
	return(NULL);
}
