/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:52:58 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/30 14:25:25 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_variable(char *clean, int j, char *ins, int i, t_macro *macro)
{
	char	*envir_name;
	char	*envir_value;

	envir_name = get_envir_name(&ins[i]);
	envir_value = get_envir_value(envir_name, macro);
	if (envir_name && envir_value)
	{
		ft_strlcpy(&clean[j], envir_value, ft_strlen(envir_value) + 1);
		j += ft_strlen(envir_value);
		i += ft_strlen(envir_name);
	}
	if (!envir_value)
		i += ft_strlen(envir_name);
	free(envir_value);
	free(envir_name);
	return (i);
}

char	*build_expanded_instruction(char *clean, char *ins, t_macro *macro)
{
	int		i;
	int		j;
	char	*exit_str;

	i = 0;
	j = 0;
	exit_str = ft_itoa(macro->exit_code);
	while (ins[i])
	{
		if (ins[i] == '$' && (ins[i + 1] == '\0' || ft_isdelim(ins[i + 1])))
			clean[j++] = ins[i++];
		else if (ft_strlen(ins) > 2 && ins[i] == '$' && ins[i + 1] != '\0' && (ft_isquote(ins[i + 1])))
		{
			clean[j++] = ins[i++];
		}
		else if (ins[i] == '$' && envir_must_be_expanded(ins, i))
		{
			i++;
			if (ins[i] == '?')
			{
				ft_strlcpy(&clean[j], exit_str, ft_strlen(exit_str) + 1);
				j += ft_strlen(exit_str);
				i++;
			}
			else
			{
				i = expand_variable(clean, j, ins, i, macro);
				j += ft_strlen(&clean[j]);
			}
		}
		else
			clean[j++] = ins[i++];
	}
	free_string(&exit_str);
	return (clean);
}

size_t	expanded_envir_len(char *ins, t_macro *macro)
{
	size_t	envir_len;
	char 	*exit_code;
	int		i;
	char	*envir_name;
	char	*envir_value;

	envir_len = 0;
	i = 0;
	while (ins[i])
	{
		if (ins[i] == '$' && (ins[i + 1] == '\0' || ft_isdelim(ins[i + 1])))
			i++;
		else if (ft_strlen(ins) > 2 && ins[i] == '$' && ft_isquote(ins[i + 1]))
		{
			i++;
			envir_len = 1;
		}
		else if (ins[i] == '$' && envir_must_be_expanded(ins, i))
		{
			i++;
			if (ins[i] == '?')
			{
				i++;
				exit_code = ft_itoa(macro->exit_code);
				envir_len += ft_strlen(exit_code);
				free_string(&exit_code);
			}
			else
			{
				envir_name = get_envir_name(&ins[i]);
				if (envir_name)
				{
					envir_value = get_envir_value(envir_name, macro);
					if (envir_value)
						envir_len += ft_strlen(envir_value);
					i += ft_strlen(envir_name);
					free_2_strings(&envir_name, &envir_value);
				}
			}
		}
		else
			i++;
	}
	return (envir_len);
}

char	*get_expanded_instruction(char *ins, t_macro *macro)
{
	char	*clean;
	size_t	envir_len;
	size_t	total_len;

	envir_len = expanded_envir_len(ins, macro);
	total_len = envir_len + ft_strlen(ins);
	clean = ft_calloc(1, sizeof(char) * total_len + 1);
	if (!clean)
		return (NULL);
	clean = build_expanded_instruction(clean, ins, macro);
	clean[total_len] = '\0';
	return (clean);
}
