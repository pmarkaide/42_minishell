/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:24:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 13:50:17 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	identify_string_tokens(t_token *tokens)
{
	int	first;

	first = 0;
	while (tokens)
	{
		if (tokens->type == STRING && first == 0)
		{
			if (is_builtin(tokens))
				tokens->type = BUILTIN;
			else
				tokens->type = CMD;
			first = 1;
		}
		else if (tokens->type == STRING && first == 1)
			tokens->type = ARG;
		if (tokens->type == PIPE)
			first = 0;
		tokens = tokens->next;
	}
}

static t_token	*identify_redirection_tokens(char *lexeme)
{
	t_token	*token;

	token = init_token();
	if (!token)
		return (NULL);
	if (ft_strncmp(lexeme, ">>", 2) == 0)
		token->type = APPEND;
	else if (ft_strncmp(lexeme, "<<", 2) == 0)
		token->type = HERE_DOC;
	else if (ft_strncmp(lexeme, ">", 1) == 0)
		token->type = OUTRED;
	else if (ft_strncmp(lexeme, "<", 1) == 0)
		token->type = INRED;
	else if (ft_strncmp(lexeme, "|", 1) == 0)
		token->type = PIPE;
	else
		token->type = STRING;
	if (token->type == APPEND || token->type == HERE_DOC)
		token->value = ft_strdup(lexeme + 2);
	else if (token->type != PIPE && token->type != STRING)
		token->value = ft_strdup(lexeme + 1);
	else
		token->value = ft_strdup(lexeme);
	return (token);
}

t_token	*identify_tokens(t_list *lexemes)
{
	t_token	*tokens;
	t_token	*token;

	tokens = NULL;
	while (lexemes)
	{
		token = identify_redirection_tokens(lexemes->content);
		if (!token)
			return (NULL);
		token_add_back(&tokens, token);
		lexemes = lexemes->next;
	}
	identify_string_tokens(tokens);
	return (tokens);
}

t_token	*build_retokens(t_list *lexemes)
{
	t_token	*token;
	t_token	*retokens;

	retokens = NULL;
	while (lexemes)
	{
		token = init_token();
		if (!token)
			return (NULL);
		if (retokens == NULL)
			token->type = CMD;
		else
			token->type = ARG;
		token->value = lexemes->content;
		token_add_back(&retokens, token);
		lexemes = lexemes->next;
	}
	return (retokens);
}

void	plug_retokens(t_token *token, t_token *retokens, t_macro *macro)
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

t_token	*retokenize(t_token *token, t_macro *macro)
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
	retokens = build_retokens(lexemes);
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
			if (tokens->type != CMD)
			{
				expanded = get_expanded_instruction(tokens->value, macro);
				if (!expanded)
					return (NULL);
				tokens->value = expanded;
			}
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
	return(macro->tokens);
}

void	tokenizer(t_macro *macro)
{
	t_list	*lexemes;

	fix_redirections(macro->instruction);
	lexemes = split_args_by_quotes(macro->instruction);
	if (!lexemes)
	{
		free_tokens(&macro->tokens);
		return ;
	}
	macro->tokens = identify_tokens(lexemes);
	if (!macro->tokens)
	{
		free(lexemes);
		return ;
	}
	ft_printf("Tokens:\n");
	print_tokens(macro->tokens);
	macro->tokens = expand_arg_tokens(macro);
	ft_printf("\n\n\n");
	ft_printf("Expanded Tokens:\n");
	print_tokens(macro->tokens);
	if (!macro->tokens)
	{
		free_tokens(&macro->tokens);
		free(lexemes);
		return ;
	}
	free(lexemes);
}
