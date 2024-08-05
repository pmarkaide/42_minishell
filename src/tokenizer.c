/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:24:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/05 14:09:25 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void identify_string_tokens(char *lexeme)
// {
// 	t_token *token;

// 	token = malloc(sizeof(t_token));

// }

static t_token	*identify_redirection_tokens(char *lexeme)
{
	t_token	*token;

	token = init_token();
	if (!token)
		return (NULL);
	if (ft_strncmp(lexeme, ">", 1) == 0)
		token->type = OUTRED;
	else if (ft_strncmp(lexeme, "<", 1) == 0)
		token->type = INRED;
	else if (ft_strncmp(lexeme, ">>", 2) == 0)
		token->type = APPEND;
	else if (ft_strncmp(lexeme, "<<", 2) == 0)
		token->type = HERE_DOC;
	else if (ft_strncmp(lexeme, "|", 1) == 0)
		token->type = PIPE;
	else
		token->type = STRING;
	if (token->type != PIPE && token->type != STRING)
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
	return (tokens);
}

static char	*clean_instruction(char *instruction)
{
	char	*clean;
	char	*ptr;
	int		i;

	clean = malloc(sizeof(char) * ft_strlen(instruction) + 1);
	ptr = instruction;
	i = 0;
	while (*ptr)
	{
		if (*ptr == ' ' && (*(ptr - 1) == '<' || *(ptr - 1) == '>'))
			ptr++;
		clean[i] = *ptr;
		i++;
		ptr++;
	}
	clean[i] = '\0';
	return (clean);
}

static char	*get_expanded_instruction(char *instruction)
{
	char	*clean;
	size_t	total_len;

	total_len = expanded_envir_len(instruction) + ft_strlen(instruction);
	clean = malloc(sizeof(char) * total_len + 1);
	if (!clean)
		return (NULL);
	clean = expand_envirs(clean, instruction);
	clean[total_len] = '\0';
	return (clean);
}

void	tokenizer(t_macro *macro)
{
	char	**lexemes;

	macro->instruction = get_expanded_instruction(macro->instruction);
	macro->instruction = clean_instruction(macro->instruction);
	lexemes = ft_split(macro->instruction, ' ');
	macro->tokens = identify_tokens(lexemes);
	print_tokens(macro->tokens);
	// while(lexemes[i])
	// {
	// 	token = identify_lexeme(lexemes[i]);
	// 	if (token)
	// 		ft_lstadd_back(&tokens, token);
	// 	i++;
	// }
	free(lexemes);
}
