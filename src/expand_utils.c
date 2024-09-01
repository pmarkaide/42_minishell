/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:53:07 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/01 21:47:26 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	envir_must_be_expanded(char *str, int index)
{
	bool	inside_single_quotes;
	bool	inside_double_quotes;
	int		i;

	inside_single_quotes = false;
	inside_double_quotes = false;
	i = 0;
	while (i < index && str[i] != '\0')
	{
		if (str[i] == '\'' && !inside_double_quotes)
			inside_single_quotes = !inside_single_quotes;
		else if (str[i] == '\"' && !inside_single_quotes)
			inside_double_quotes = !inside_double_quotes;
		i++;
	}
	if (inside_single_quotes)
		return (false);
	else if (inside_double_quotes)
		return (true);
	else
		return (true);
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

void	handle_delimiter_after_dollar(char **clean, char *ins, size_t *i)
{
	char	str[2];
	char	*temp;

	str[0] = ins[*i];
	str[1] = '\0';
	temp = ft_strjoin(*clean, str, NULL);
	if (!temp)
		return (free(*clean));
	free_string(clean);
	*clean = temp;
	(*i)++;
}

void	handle_unexpected_case(char **clean, char *ins, size_t *i)
{
	char	str[2];
	char	*temp;

	str[0] = ins[*i];
	str[1] = '\0';
	temp = ft_strjoin(*clean, str, NULL);
	if (!temp)
		return (free(*clean));
	free_string(clean);
	*clean = temp;
	(*i)++;
}
