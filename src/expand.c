/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:52:58 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/03 23:28:57 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_envir(char **clean, char *ins, size_t *i, t_macro *macro)
{
	size_t	start;
	size_t	len;
	char	*envir_name;
	char	*envir_value;

	start = *i + 1;
	while (ft_isalnum(ins[start]) || ins[start] == '_')
		start++;
	len = start - (*i + 1);
	envir_name = ft_substr(ins, *i + 1, len);
	if (!envir_name)
		return (free(*clean));
	envir_value = ft_getenv(envir_name, macro->env);
	free_string(&envir_name);
	if (envir_value)
		*clean = ft_strjoin(*clean, envir_value, NULL, 3);
	*i = start;
}

void	handle_exit_code(char **clean, size_t *i, t_macro *macro)
{
	char	*substr;

	substr = ft_itoa(macro->exit_code);
	if (!substr)
		free_string(clean);
	*clean = ft_strjoin(*clean, substr, NULL, 3);
	*i += 2;
}

static void	literal_cases(char **clean, char *ins, size_t *i)
{
	if (ins[*i] != '$' || !envir_must_be_expanded(ins, *i))
		handle_normal_char(clean, ins, i);
	else if (ins[*i] == '$' && ft_isquote(ins[*i + 1]))
	{
		if (inside_double_quotes(ins, *i))
			handle_quoted_literal(clean, ins, i);
		else
			(*i)++;
	}
	else if (ins[*i] == '$' && ft_isdelim(ins[*i + 1]))
		handle_delimiter_after_dollar(clean, ins, i);
}

static void	dollar_cases(char **clean, char *ins, size_t *i, t_macro *macro)
{
	if (ins[*i] == '$' && ins[*i + 1] == '?')
		handle_exit_code(clean, i, macro);
	else if (ins[*i] == '$' && (ft_isalnum(ins[*i + 1]) || ins[*i + 1] == '_'))
		handle_envir(clean, ins, i, macro);
	else
		handle_unexpected_case(clean, ins, i);
}

char	*get_expanded_ins(char *ins, t_macro *macro)
{
	char	*clean;
	size_t	i;

	clean = ft_strdup("");
	if (!clean)
		return (NULL);
	i = 0;
	while (ins[i])
	{
		literal_cases(&clean, ins, &i);
		if (!clean)
			return (NULL);
		dollar_cases(&clean, ins, &i, macro);
		if (!clean)
			return (NULL);
	}
	return (clean);
}
