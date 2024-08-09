/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:42:50 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/09 21:54:03 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(t_token *token)
{
	if (token->type == INRED)
		return (true);
	else if (token->type == OUTRED)
		return (true);
	else if (token->type == HERE_DOC)
		return (true);
	else if (token->type == APPEND)
		return (true);
	return (false);
}

bool	is_builtin(t_token *token)
{
	if (ft_strcmp(token->value, "echo") == 0)
		return (true);
	if (ft_strcmp(token->value, "cd") == 0)
		return (true);
	if (ft_strcmp(token->value, "pwd") == 0)
		return (true);
	if (ft_strcmp(token->value, "export") == 0)
		return (true);
	if (ft_strcmp(token->value, "unset") == 0)
		return (true);
	if (ft_strcmp(token->value, "env") == 0)
		return (true);
	if (ft_strcmp(token->value, "exit") == 0)
		return (true);
	return (false);
}

bool	is_inside_single_quotes(const char *str, int index)
{
	bool	inside_single_quotes;
	int		i;

	inside_single_quotes = false;
	i = 0;
	while (i <= index && str[i] != '\0')
	{
		if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\'))
			inside_single_quotes = !inside_single_quotes;
		i++;
	}
	return (inside_single_quotes);
}

char	*get_envir_value(const char *str, int *len)
{
	char	*envir_name;
	char	*envir_value;

	*len = 0;
	while (ft_isalnum(str[*len]) || str[*len] == '_')
		(*len)++;
	envir_name = (char *)malloc(*len + 1);
	if (envir_name)
	{
		ft_strncpy(envir_name, str, *len);
		envir_name[*len] = '\0';
	}
	else
		return (NULL);
	envir_value = getenv(envir_name);
	free(envir_name);
	return (envir_value);
}

size_t	expanded_envir_len(char *instruction)
{
	size_t	len;
	int		i;
	char	*ptr;
	char	*envir_value;
	int		envir_name_len;

	if (!instruction)
		return (0);

	len = 0;
	i = 0;
	ptr = instruction;
	while (ptr[i])
	{
		if (ptr[i] == '$' && !is_inside_single_quotes(ptr, i))
		{
			 if (ptr[i + 1] == '\0')
                break;
			envir_value = get_envir_value(&ptr[i + 1], &envir_name_len);
			if (envir_value)
				len += ft_strlen(envir_value);
			i += envir_name_len + 1;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*expand_envirs(char *clean, char *instruction)
{
	char	*ptr;
	int		i;
	int		len;
	char	*envir_value;

	ptr = instruction;
	i = 0;
	while (*ptr)
	{
		if (*ptr != '$')
			clean[i++] = *ptr++;
		else
		{
			ptr++;
			envir_value = get_envir_value(ptr, &len);
			if (envir_value)
			{
				ft_strlcpy(&clean[i], envir_value, len + 1);
				i += len;
				free(envir_value);
			}
			ptr += len;
		}
	}
	return (clean);
}
