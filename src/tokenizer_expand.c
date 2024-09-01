/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:45:23 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/01 13:22:41 by pmarkaid         ###   ########.fr       */
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
	token->next = NULL;
	free_tokens(&token);
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

char	*handle_literal_string(t_token *token, char *expanded)
{
	char	*copy;

	free_string(&token->value);
	copy = ft_strdup(expanded);
	if (!copy)
		return (NULL);
	return (copy);
}

t_token	*handle_retokenize(char *expanded, t_token *tokens, t_macro *macro)
{
	t_token	*retokens;
	t_token	*last_retokens;

	retokens = retokenize(expanded, tokens);
	if (!retokens)
		return (NULL);
	last_retokens = plug_retokens(tokens, retokens, macro);
	return (last_retokens);
}

char	*expand_token(t_token *token, t_macro *macro)
{
	char	*expanded;

	expanded = get_expanded_instruction(token->value, macro);
	if (!expanded || *expanded == '\0')
		return (NULL);
	if (ft_strchr("\"", token->value[0]))
	{
		token->value = handle_literal_string(token, expanded);
		if (!token->value)
			return (free_string(&expanded));
	}
	else
	{
		token = handle_retokenize(expanded, token, macro);
		if (!token)
			return (free_string(&expanded));
	}
	free_string(&expanded);
	return (token->value);
}

t_token	*expand_arg_tokens(t_macro *macro)
{
	t_token	*tokens;

	tokens = macro->tokens;
	while (tokens && tokens->type != HERE_DOC)
	{
		if (ft_strchr(tokens->value, '$'))
		{
			if (!expand_token(tokens, macro))
				return (NULL);
		}
		if (tokens == NULL || tokens->next == NULL)
			return (NULL);
		tokens = tokens->next;
	}
	return (macro->tokens);
}

t_token	*remove_empty_envir_tokens(t_macro *macro)
{
	t_token	*tokens;
	char	*expanded;

	tokens = macro->tokens;
	while (tokens)
	{
		if (ft_strchr(tokens->value, '$'))
		{
			expanded = get_expanded_instruction(tokens->value, macro);
			if (!expanded)
			{
				free_tokens(&tokens);
				return (NULL);
			}
			if (*expanded == '\0')
				remove_token(&macro->tokens, tokens);
			free_string(&expanded);
		}
		tokens = tokens->next;
	}
	return (macro->tokens);
}

void	ensure_at_least_one_cmd(t_token **tokens)
{
	t_token	*current;
	int		cmd_found;
	int		first_token;

	current = *tokens;
	cmd_found = 0;
	first_token = 1;
	while (current)
	{
		if (first_token || current->type == PIPE)
		{
			cmd_found = 0;
			first_token = 0;
		}
		if (is_redir(current, "infile") || is_redir(current, "outfile"))
		{
			current = current->next;
			continue ;
		}
		if (current->type == CMD)
			cmd_found = 1;
		if (!cmd_found && current->type == ARG)
		{
			current->type = CMD;
			cmd_found = 1;
		}
		current = current->next;
	}
}
