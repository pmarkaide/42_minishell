/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:52:58 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/31 21:51:35 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_clean_to_null(char *clean)
{
	free_string(&clean);
}

void	handle_normal_char(char **clean, char *ins, size_t *i)
{
	char	str[2] = {ins[*i], '\0'};
	char	*temp;

	temp = ft_strjoin(*clean, str, NULL);
	if (!temp)
		return (set_clean_to_null(*clean));
	free_string(clean);
	*clean = temp;
	(*i)++;
}

char	*expand_exit_code(t_macro *macro)
{
	char	*exit_code;

	exit_code = ft_itoa(macro->exit_code);
	if (!exit_code)
		return (NULL);
	return (exit_code);
}

void	handle_exit_code(char **clean, size_t *i, t_macro *macro)
{
	char	*substr;
	char	*temp;

	substr = expand_exit_code(macro);
	if (!substr)
		return (set_clean_to_null(*clean));
	temp = ft_strjoin(*clean, substr, NULL);
	if (!temp)
		return (set_clean_to_null(*clean));
	free_string(clean);
	*clean = temp;
	*i += 2;
	free(substr);
}

void	handle_delimiter_after_dollar(char **clean, char *ins, size_t *i)
{
	char	str[2] = {ins[*i], '\0'};
	char	*temp;

	temp = ft_strjoin(*clean, str, NULL);
	if (!temp)
		return (set_clean_to_null(*clean));
	free_string(clean);
	*clean = temp;
	(*i)++;
}

void	handle_envir(char **clean, char *ins, size_t *i, t_macro *macro)
{
	char	*envir_name;
	size_t	envir_name_len;
	char	*envir_value;
	char	*res;

	envir_name = get_envir_name(&ins[*i + 1]);
	if (!envir_name)
        return (set_clean_to_null(*clean));
	envir_name_len = ft_strlen(envir_name);
	envir_value = get_envir_value(envir_name, macro);
	free(envir_name);
	if (!envir_value)
	{
		envir_value = ft_strdup("");
		if (!envir_value)
            return (set_clean_to_null(*clean));
	}
	res = ft_strjoin(*clean, envir_value, NULL);
	free(envir_value);
	if (!res)
        return (set_clean_to_null(*clean));
	free(*clean);
	*clean = res;
	*i += envir_name_len + 1;
}

void	handle_quoted_literal(char **clean, char *ins, size_t *i)
{
	char	quote_char;
	char	*quote_start;
	char	*quote_end;
	size_t	quote_len;
	char	*literal;
	char	*temp;

	quote_char = ins[*i + 1];
	(*i) += 2;
	quote_start = &ins[*i];
	quote_end = strchr(quote_start, quote_char);
	if (quote_end != NULL)
	{
		quote_len = quote_end - quote_start;
		literal = ft_substr(ins, *i - 2, quote_len + 2);
        if(!literal)
            return (set_clean_to_null(*clean));
		temp = ft_strjoin(*clean, literal, NULL);
		if (!temp)
			return (set_clean_to_null(*clean));
		free_string(clean);
		*clean = temp;
		*i += quote_len + 1;
		free_string(&literal);
	}
	else
	{
		temp = ft_strjoin(*clean, &ins[*i - 2], NULL);
		if (!temp)
			return (set_clean_to_null(*clean));
		free_string(clean);
		*clean = temp;
		*i += strlen(&ins[*i]);
	}
}

void	handle_unexpected_case(char **clean, char *ins, size_t *i)
{
	char	str[2] = {ins[*i], '\0'};
	char	*temp;

	temp = ft_strjoin(*clean, str, NULL);
	if (!temp)
        return (set_clean_to_null(*clean));
	free(*clean);
	*clean = temp;
	(*i)++;
}

char	*get_expanded_instruction(char *ins, t_macro *macro)
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
		else if (ins[i] == '$' && ft_isalpha(ins[i + 1]))
			handle_envir(&clean, ins, &i, macro);
		else
			handle_unexpected_case(&clean, ins, &i);
		if (!clean)
			return (NULL);
	}
	return (clean);
}
