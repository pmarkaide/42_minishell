/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:52:58 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 16:06:03 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_variable(char *clean, int j, char *instruction, int i,
		t_macro *macro)
{
	char	*envir_name;
	char	*envir_value;

	envir_name = get_envir_name(&instruction[i]);
	envir_value = get_envir_value(envir_name, macro);
	if (envir_name && envir_value)
	{
		ft_strlcpy(&clean[j], envir_value, ft_strlen(envir_value) + 1);
		j += ft_strlen(envir_value);
		i += ft_strlen(envir_name);
		free(envir_value);
		free(envir_name);
	}
	return (i);
}

char	*build_expanded_instruction(char *clean, char *instruction,
		t_macro *macro)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (instruction[i])
	{
		if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			i++;
			i = expand_variable(clean, j, instruction, i, macro);
			j += ft_strlen(&clean[j]);
		}
		else
			clean[j++] = instruction[i++];
	}
	clean[j] = '\0';
	return (clean);
}

size_t	expanded_envir_len(char *instruction, t_macro *macro)
{
	size_t	envir_len;
	int		i;
	char	*envir_name;
	char	*envir_value;

	envir_len = 0;
	i = 0;
	while (instruction[i])
	{
		if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			i++;
			envir_name = get_envir_name(&instruction[i]);
			if (envir_name)
			{
				envir_value = get_envir_value(envir_name, macro);
				if (envir_value)
					envir_len += ft_strlen(envir_value);
				i += ft_strlen(envir_name);
				free(envir_name);
			}
		}
		else
			i++;
	}
	return (envir_len);
}

char	*get_expanded_instruction(char *instruction, t_macro *macro)
{
	char	*clean;
	size_t	envir_len;
	size_t	total_len;

	envir_len = expanded_envir_len(instruction, macro);
	total_len = envir_len + ft_strlen(instruction);
	clean = calloc(1, sizeof(char) * total_len + 1);
	if (!clean)
		return (NULL);
	clean = build_expanded_instruction(clean, instruction, macro);
	clean[total_len] = '\0';
	return (clean);
}
