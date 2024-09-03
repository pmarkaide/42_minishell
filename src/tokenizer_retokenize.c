/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_retokenize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:56:54 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/03 15:05:48 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*relabel_token(t_token *token, t_type *type, t_list *lexemes)
{
    if (*type == CMD)
    {
        token->type = CMD;
        *type = ARG;
    }
    else if (*type == ARG)
        token->type = ARG;
    else if (type_is_redirection(*type))
    {
        token->type = *type;
        if (lexemes->next)
        {
            ft_putstr_fd("minishell: ambiguous redirect\n", 2);
            return (NULL);
        }
    }
    else
        token->type = ARG;
    return (token);
}


static t_token	*build_retokens(t_list *lexemes, t_type type)
{
    t_token	*token;
    t_token	*retokens;

    retokens = NULL;
    while (lexemes)
    {
        token = init_token();
        if (!token)
            return (NULL);
        
        token = relabel_token(token, &type, lexemes);
        if (!token)
            return (free_tokens(&retokens));
        
        token->value = ft_strdup(lexemes->content);
        if (!token->value)
            return (free_tokens(&token));
        
        token_add_back(&retokens, token);
        lexemes = lexemes->next;
    }
    return (retokens);
}

static t_token	*plug_retokens(t_token *token, t_token *retokens,
		t_macro *macro)
{
	t_token	*next;
	t_token	*prev;
	t_token	*last;

	next = token->next;
	prev = macro->tokens;
	while (prev && prev->next != token)
		prev = prev->next;
	if (prev)
		prev->next = retokens;
	else
		macro->tokens = retokens;
	last = last_token(retokens);
	if (last)
		last->next = next;
	return (last);
}

static t_token	*retokenize(char *expanded, t_token *token)
{
	t_list	*lexemes;
	t_token	*retokens;

	lexemes = split_args_by_quotes(expanded);
	if (!lexemes)
		return (NULL);
	retokens = build_retokens(lexemes, token->type);
	if (!retokens)
		return (NULL);
	ft_lstclear(&lexemes, ft_del);
	return (retokens);
}

t_token	*handle_retokenize(char *expanded, t_token *token, t_macro *macro)
{
	t_token	*retokens;
	t_token	*last_retokens;

	retokens = retokenize(expanded, token);
	if (!retokens)
		return (NULL);
	last_retokens = plug_retokens(token, retokens, macro);
	free_string(&token->value);
	free(token);
	return (last_retokens);
}
