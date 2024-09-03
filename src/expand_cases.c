/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 23:27:34 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/03 23:30:51 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_normal_char(char **clean, char *ins, size_t *i)
{
	char	str[2];

	str[0] = ins[*i];
	str[1] = '\0';
	*clean = ft_strjoin(*clean, str, NULL, 1);
	(*i)++;
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
		temp = ft_substr(ins, *i, quote_end - quote_start + 3);
		*i += (quote_end - quote_start) + 3;
	}
	else
	{
		temp = ft_strdup(&ins[*i]);
		*i += ft_strlen(&ins[*i]);
	}
	if (!temp)
	{
		free(*clean);
		return ;
	}
	*clean = ft_strjoin(*clean, temp, NULL, 3);
}

void	handle_delimiter_after_dollar(char **clean, char *ins, size_t *i)
{
	char	str[2];
	char	*temp;

	str[0] = ins[*i];
	str[1] = '\0';
	temp = ft_strjoin(*clean, str, NULL, 1);
	if (!temp)
		return ;
	*clean = temp;
	(*i)++;
}

void	handle_unexpected_case(char **clean, char *ins, size_t *i)
{
	char	str[2];
	char	*temp;

	str[0] = ins[*i];
	str[1] = '\0';
	temp = ft_strjoin(*clean, str, NULL, 1);
	if (!temp)
		return ;
	*clean = temp;
	(*i)++;
}
