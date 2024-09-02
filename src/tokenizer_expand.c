/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:45:23 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/02 21:57:13 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_literal_string(t_token *token, char *expanded)
{
	char	*copy;

	free_string(&token->value);
	copy = ft_strdup(expanded);
	if (!copy)
		return (NULL);
	return (copy);
}

char	*expand_token(t_token *token, t_macro *macro)
{
	char	*expanded;

	expanded = get_expanded_ins(token->value, macro);
	if (!expanded || *expanded == '\0')
		return (NULL);
	if (ft_strchr("\"", token->value[0]))
	{
		token->value = handle_literal_string(token, expanded);
		if (!token->value)
		{
			token = NULL;
			return (free_string(&expanded));
		}
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
		if (tokens == NULL)
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
			expanded = get_expanded_ins(tokens->value, macro);
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
		if (current->type == CMD || current->type == BUILTIN)
			cmd_found = 1;
		if (!cmd_found && current->type == ARG)
		{
			current->type = CMD;
			cmd_found = 1;
		}
		current = current->next;
	}
}
