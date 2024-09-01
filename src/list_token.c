/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 13:44:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/01 13:45:26 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	ft_bzero(token, sizeof(t_token));
	return (token);
}

void	token_add_back(t_token **tokens, t_token *new)
{
	if (!tokens || !new)
		return ;
	if (!*tokens)
		*tokens = new;
	else
		(last_token(*tokens))->next = new;
}

void	remove_token(t_token **tokens, t_token *token)
{
	t_token	*current;
	t_token	*prev;

	current = *tokens;
	prev = NULL;
	while (current != NULL)
	{
		if (current == token)
		{
			if (prev != NULL)
				prev->next = current->next;
			else
				*tokens = current->next;
			free_string(&current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

t_token	*last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

int	tokens_size(t_token *tokens)
{
	size_t	i;

	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	return ((int)i);
}
