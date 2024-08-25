/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:45:23 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/23 13:21:31 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (type == CMD)
		{
			token->type = CMD;
			type = ARG;
		}
		else
			token->type = ARG;
		token->value = lexemes->content;
		token_add_back(&retokens, token);
		lexemes = lexemes->next;
	}
	return (retokens);
}

static void	plug_retokens(t_token *token, t_token *retokens, t_macro *macro)
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
}

static t_token	*retokenize(t_token *token, t_macro *macro)
{
	t_list	*lexemes;
	t_token	*retokens;
	char	*expanded;

	expanded = get_expanded_instruction(token->value, macro);
	if (!expanded)
		return (NULL);
	lexemes = split_args_by_quotes(expanded);
	if (!lexemes)
		return (NULL);
	retokens = build_retokens(lexemes, token->type);
	return (retokens);
}

t_token	*expand_arg_tokens(t_macro *macro)
{
	t_token	*tokens;
	t_token	*retokens;
	char	*expanded;

	tokens = macro->tokens;
	while (tokens)
	{
		if (ft_strchr(tokens->value, '$'))
		{
			expanded = get_expanded_instruction(tokens->value, macro);
            if (!expanded || *expanded == '\0')
            {
                tokens->value = ft_strdup("");
                if (!tokens->value)
                    return (NULL);
            }
			if (ft_strchr("\"", tokens->value[0]))
				tokens->value = expanded;
			else
			{
				retokens = retokenize(tokens, macro);
				if (!retokens)
					return (NULL);
				plug_retokens(tokens, retokens, macro);
			}
		}
		tokens = tokens->next;
	}

	// Promotion logic
    if (macro->tokens && macro->tokens->value[0] == '\0' && macro->tokens->next)
    {
        t_token *second_token = macro->tokens->next;
        macro->tokens->value = second_token->value;
        macro->tokens->next = second_token->next;
        free(second_token);
    }
	return (macro->tokens);
}
