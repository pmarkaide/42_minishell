/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 14:00:34 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/01 14:01:10 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_normal_char(char **clean, char *ins, size_t *i)
{
	char	str[2] = {ins[*i], '\0'};
	char	*temp;

	temp = ft_strjoin(*clean, str, NULL);
	if (!temp)
		return (free(*clean));
	free_string(clean);
	*clean = temp;
	(*i)++;
}

void	handle_exit_code(char **clean, size_t *i, t_macro *macro)
{
	char	*substr;
	char	*temp;

	substr = expand_exit_code(macro);
	if (!substr)
		return (free(*clean));
	temp = ft_strjoin(*clean, substr, NULL);
	if (!temp)
		return (free(*clean));
	free_string(clean);
	*clean = temp;
	*i += 2;
	free_string(&substr);
}

void	handle_delimiter_after_dollar(char **clean, char *ins, size_t *i)
{
	char	str[2] = {ins[*i], '\0'};
	char	*temp;

	temp = ft_strjoin(*clean, str, NULL);
	if (!temp)
		return (free(*clean));
	free_string(clean);
	*clean = temp;
	(*i)++;
}
