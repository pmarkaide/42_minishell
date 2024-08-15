/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:39:10 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/16 00:06:34 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Transverse the original instruction
 * If envir surrounded by S quotes, continue
 * If envir surrounded by double quotes, expand, maintaining the quotes
 * If envirs outside quotes, expand without quotes
 * Tokenize the new instruction
 * Continue parsing, execution etc.
 */

static char	*clean_instruction(char *instruction)
{
	char	*clean;
	char	*ptr;
	int		i;

	clean = malloc(sizeof(char) * ft_strlen(instruction) + 1);
	ptr = instruction;
	i = 0;
	while (*ptr)
	{
		// Riesgo de Segfault si da la casualidad de que ptr[0] es espacio e intenta leer ptr[-1]
		if (*ptr == ' ' && (*(ptr - 1) == '<' || *(ptr - 1) == '>'))
			ptr++;
		clean[i] = *ptr;
		i++;
		ptr++;
	}
	clean[i] = '\0';
	free(instruction);
	return (clean);
}

char	*build_expanded_instruction(char *clean, char *instruction, t_macro *macro)
{
	int		i;
	char	*envir_value;
	char	*envir_name;

	i = 0;
	while (*instruction)
	{
		if (*instruction != '$')
			clean[i++] = *instruction++;
		else
		{
			instruction++;
			envir_name = get_envir_name(instruction);
			envir_value = get_envir_value(instruction, macro);
			if (envir_name && envir_value)
			{
				ft_strlcpy(&clean[i], envir_value, ft_strlen(envir_value) + 1);
				i += ft_strlen(envir_value);
				free(envir_value);
				instruction += ft_strlen(envir_name);
			}
		}
	}
	clean[i] = '\0';
	return (clean);
}

size_t	expanded_instruction_len(char *instruction, t_macro *macro)
{
	size_t	len;
	int		i;
	char	*envir_value;

	len = 0;
	i = 0;
	while (instruction[i])
	{
		if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			i++;
			envir_value = get_envir_value(&instruction[i], macro);
			if (envir_value)
				len += ft_strlen(envir_value);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

static char	*get_expanded_instruction(char *instruction, t_macro *macro)
{
	char	*clean;
	int		env_len;
	size_t	total_len;

	env_len = expanded_instruction_len(instruction, macro);
	total_len = env_len + ft_strlen(instruction);
	clean = calloc(1, sizeof(char) * total_len + 1);
	if (!clean)
		return (NULL);
	clean = build_expanded_instruction(clean, instruction, macro);
	clean[total_len] = '\0';
	free(instruction);
	return (clean);
}

void clean(t_macro *macro)
{
	macro->instruction = get_expanded_instruction(macro->instruction, macro);
	macro->instruction = clean_instruction(macro->instruction);
}
