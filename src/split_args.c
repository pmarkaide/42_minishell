/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:47:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 15:41:55 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_and_add_node(char *start, size_t length, t_list **tokens)
{
	char	*token;
	t_list	*new_node;

	token = ft_strndup(start, length);
	if (!token)
		return (-1);
	new_node = ft_lstnew(token);
	if (!new_node)
	{
		free(token);
		return (-1);
	}
	ft_lstadd_back(tokens, new_node);
	return (0);
}

static void	process_quote(char *ins, int *pos, int *len)
{
	char	quote_char;

	quote_char = ins[*pos];
	(*pos)++;
	(*len)++;
	while (ins[*pos] && ins[*pos] != quote_char)
	{
		(*len)++;
		(*pos)++;
	}
	if (ins[*pos] == quote_char)
	{
		(*len)++;
		(*pos)++;
	}
}

static int	process_pipe(char *ins, int *pos, t_list **tokens, char **start,
		int *len)
{
	if (*len > 0)
	{
		if (create_and_add_node(*start, *len, tokens) == -1)
			return (-1);
	}
	if (create_and_add_node("|", 1, tokens) == -1)
		return (-1);
	(*pos)++;
	*start = &ins[*pos];
	*len = 0;
	return (0);
}

static void	process_character(int *pos, int *len)
{
	(*len)++;
	(*pos)++;
}

static int	process_token(char *ins, int *pos, t_list **tokens)
{
	char	*start;
	int		len;
	int		result;

	start = &ins[*pos];
	len = 0;
	result = 0;
	while (ins[*pos] && !ft_isdelim(ins[*pos]))
	{
		if (ft_isquote(ins[*pos]))
			process_quote(ins, pos, &len);
		else if (ins[*pos] == '|')
			result = process_pipe(ins, pos, tokens, &start, &len);
		else
			process_character(pos, &len);
		if (result == -1)
			return (-1);
	}
	if (len > 0)
	{
		if (create_and_add_node(start, len, tokens) == -1)
			return (-1);
	}
	return (0);
}

t_list	*split_args_by_quotes(char *ins)
{
	t_list	*lexemes;
	int		pos;

	lexemes = NULL;
	pos = 0;
	while (ins[pos])
	{
		while (ins[pos] && ft_isdelim(ins[pos]))
			pos++;
		if (ins[pos])
		{
			if (process_token(ins, &pos, &lexemes) == -1)
			{
				ft_lstclear(&lexemes, ft_del);
				return (NULL);
			}
		}
	}
	return (lexemes);
}
