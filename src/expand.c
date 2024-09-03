/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:52:58 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/03 10:46:40 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_normal_char(char **clean, char *ins, size_t *i)
{
	char	str[2];
	char	*temp;

	str[0] = ins[*i];
	str[1] = '\0';
	temp = ft_strjoin(*clean, str, NULL, 1);
	*clean = temp;
	(*i)++;
}

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

void	handle_quoted_literal(char **clean, char *ins, size_t *i)
{
	char	quote_char;
	char	*quote_start;
	char	*quote_end;
	char	*temp;

	quote_char = ins[*i + 1];
	quote_start = &ins[*i + 2];
	quote_end = ft_strchr(quote_start, quote_char);
	if (quote_end)
	{
		temp = ft_substr(ins, *i, quote_end - ins - *i + 1);
		*i += (quote_end - quote_start) + 2;
	}
	else
	{
		temp = ft_strdup(&ins[*i]);
		*i += ft_strlen(&ins[*i]);
	}
	if (!temp)
	{
		free(*clean);
		*clean = NULL;
		return;
	}
	*clean = ft_strjoin(*clean, temp, NULL, 3);
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
		if (ins[i] != '$' || !envir_must_be_expanded(ins, i))
			handle_normal_char(&clean, ins, &i);
		else if (ins[i] == '$' && ins[i + 1] == '?')
			handle_exit_code(&clean, &i, macro);
		else if (ins[i] == '$' && (ins[i + 1] == '\'' || ins[i + 1] == '\"'))
			handle_quoted_literal(&clean, ins, &i);
		else if (ins[i] == '$' && ft_isdelim(ins[i + 1]))
			handle_delimiter_after_dollar(&clean, ins, &i);
		else if (ins[i] == '$' && (ft_isalnum(ins[i + 1]) || ins[i + 1] == '_'))
			handle_envir(&clean, ins, &i, macro);
		else
			handle_unexpected_case(&clean, ins, &i);
		if (!clean)
			return (NULL);
	}
	return (clean);
}
