/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:39:10 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/16 01:08:35 by pmarkaid         ###   ########.fr       */
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

int expand_variable(char *clean, int j, char *instruction, int i, t_macro *macro)
{
    char *envir_name;
    char *envir_value;

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

char *build_expanded_instruction(char *clean, char *instruction, t_macro *macro)
{
    int i;
    int j;

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

size_t expanded_envir_len(char *instruction, t_macro *macro)
{
    size_t envir_len;
    int i;
    char *envir_name;
    char *envir_value;

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
    return envir_len;
}

static char	*get_expanded_instruction(char *instruction, t_macro *macro)
{
	char	*clean;
	size_t envir_len;
	size_t	total_len;

	envir_len = expanded_envir_len(instruction, macro);
	total_len =  envir_len + ft_strlen(instruction);
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
