/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:43:43 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/04 12:51:50 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *init_token(void)
{
	t_token *token;

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

t_token	*last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

void free_tokens(t_token **tokens)
{
    t_token *tmp;

    while (*tokens)
    {
        tmp = *tokens;
        *tokens = (*tokens)->next;
        free(tmp->value);
        free(tmp);
    }
    *tokens = NULL;
}
char *enum_to_char(t_type type)
{
	if (type == STRING)
		return ("STRING");
	else if (type == CMD)
		return ("CMD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == INRED)
		return ("INRED");
	else if (type == OUTRED)
		return ("OUTRED");
	else if (type == HERE_DOC)
		return ("HERE_DOC");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == NEWLINE)
		return ("NEWLINE");
	return (NULL);
}

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("%s: %s\n", enum_to_char(tokens->type), tokens->value);
		tokens = tokens->next;
	}
}
