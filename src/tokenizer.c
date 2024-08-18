/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:24:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/16 09:58:46 by pmarkaid         ###   ########.fr       */
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

t_token	*identify_tokens(char **lexemes)
{
	t_token	*tokens;
	t_token	*token;
	int		i;

	i = 0;
	tokens = NULL;
	while (lexemes[i])
	{
		token = identify_redirection_tokens(lexemes[i]);
		if (!token)
			return (NULL);
		token_add_back(&tokens, token);
		i++;
	}
	identify_string_tokens(tokens);
	return (tokens);
}



void	tokenizer(t_macro *macro)
{
	char	**lexemes;

	lexemes = ft_split_args(macro->instruction);
	macro->tokens = identify_tokens(lexemes);
	print_tokens(macro->tokens);
	return;
	free(lexemes);
}
