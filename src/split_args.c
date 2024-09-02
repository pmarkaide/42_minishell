/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:47:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/02 16:31:17 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_and_add_node(char *str, size_t length, t_list **tokens)
{
	char	*token;
	t_list	*new_node;

	token = ft_strndup(str, length);
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

static int	get_next_token_len(char *str)
{
	int		len;
	char	quote_char;

	len = 0;
	if (!str || !*str)
		return (0);
	if (*str == '|')
		return (1);
	else
		while (str[len] && !ft_isdelim(str[len]) && str[len] != '|')
		{
			if (str[len] == '\'' || str[len] == '\"')
			{
				quote_char = str[len];
				len++;
				while (str[len] && str[len] != quote_char)
					len++;
				if (str[len] == quote_char)
					len++;
			}
			len++;
		}
	return (len);
}

t_list	*split_args_by_quotes(char *ins)
{
	t_list	*lexemes;
	int		len;

	lexemes = NULL;
	while (*ins)
	{
		//ft_printf("char is: %c\n", *ins);
		len = get_next_token_len(ins);
		if (len <= 0)
		{
			ft_lstclear(&lexemes, ft_del);
			return (NULL);
		}
		if (len == 0)
			break ;
		if (create_and_add_node(ins, len, &lexemes) == -1)
		{
			ft_lstclear(&lexemes, ft_del);
			return (NULL);
		}
		ins += len;
		while (*ins && ft_isdelim(*ins))
			ins++;
	}
	return (lexemes);
}
