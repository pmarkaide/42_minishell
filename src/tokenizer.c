/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:24:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/01 13:33:48 by pmarkaid         ###   ########.fr       */
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
	t_list	*tmp;

	tokens = NULL;
	tmp = lexemes;
	while (tmp)
	{
		token = identify_redirection_tokens(tmp->content);
		if (!token)
		{
			free_tokens(&tokens);
			return (NULL);
		}
		token_add_back(&tokens, token);
		tmp = tmp->next;
	}
	ft_lstclear(&lexemes, ft_del);
	lexemes = NULL;
	identify_string_tokens(tokens);
	return (tokens);
}

int	tokenizer(t_macro *macro)
{
	t_list	*lexemes;

	fix_redirections(macro->ins);
	lexemes = split_args_by_quotes(macro->ins);
	if (!lexemes)
		return (-1);
	macro->tokens = identify_tokens(lexemes);
	if (!macro->tokens)
		return (-1);
	macro->tokens = remove_empty_envir_tokens(macro);
	if (!macro->tokens)
		return (-1);
	ensure_at_least_one_cmd(&macro->tokens);
	macro->tokens = expand_arg_tokens(macro);
	if (!macro->tokens)
	{
		free_tokens(&macro->tokens);
		return (-1);
	}
	if (clean_token_quotes(macro->tokens) == (-1))
		return (-1);
	return (0);
}
