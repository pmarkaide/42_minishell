/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:53:07 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 20:55:07 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_inside_double_quotes(char *str, int index)
{
	bool	inside_double_quotes;
	int		i;

	inside_double_quotes = false;
	i = 0;
	while (i <= index && str[i] != '\0')
	{
		if (str[i] == '\"')
			inside_double_quotes = !inside_double_quotes;
		i++;
	}
	return (inside_double_quotes);
}

bool	envir_must_be_expanded(char *str, int index)
{
	if (!is_in_quote(str, index))
		return (true);
	if (is_inside_double_quotes(str, index))
		return (true);
	return (false);
}

bool	is_in_quote(char *str, int index)
{
	bool	inside_single_quotes;
	bool	inside_double_quotes;
	int		i;

	inside_single_quotes = false;
	inside_double_quotes = false;
	i = 0;
	while (i <= index && str[i] != '\0')
	{
		if (str[i] == '\'')
			inside_single_quotes = !inside_single_quotes;
		if (str[i] == '\"')
			inside_double_quotes = !inside_double_quotes;
		i++;
	}
	if (inside_single_quotes || inside_double_quotes)
		return (true);
	return (false);
}

char	*get_envir_name(char *str)
{
	int		i;
	char	*envir_name;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	envir_name = ft_substr(str, 0, i);
	if (!envir_name)
		return (NULL);
	return (envir_name);
}

char	*get_envir_value(char *str, t_macro *macro)
{
	char	*envir_name;
	char	*envir_value;

	envir_name = get_envir_name(str);
	if (!envir_name)
		return (NULL);
	envir_value = ft_getenv(envir_name, macro->env);
	free(envir_name);
	if (!envir_value)
		return (NULL);
	return (envir_value);
}
