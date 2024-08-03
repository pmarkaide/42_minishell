/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:24:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/03 22:21:11 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void identify_string_tokens(char *lexeme)
{
	t_token *token;

	token = malloc(sizeof(t_token));

}

//TODO: append and here_doc need also a lexeme + 1
static t_token *identify_rediretion_tokens(char *lexeme)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (ft_strncmp(lexeme, ">", 1) == 0)
	{
		token->type = OUTRED;
		token->value = ft_strdup(lexeme + 1);
	}
	else if (ft_strncmp(lexeme, "<", 1) == 0)
	{
		token->type = INRED;
		token->value = ft_strdup(lexeme + 1);
	}
	else if (ft_strncmp(lexeme, "|", 1) == 0)
		token->type = PIPE;
	else if (ft_strncmp(lexeme, ">>", 2) == 0)
		token->type = APPEND;
	else if (ft_strncmp(lexeme, "<<", 2) == 0)
		token->type = HERE_DOC;
	else
		token->type = STRING;
	token->value = ft_strdup(lexeme);
	return (token);
}

static t_token *identify_lexeme(char **lexeme)
{
	t_token *token;

	// identify_rediretion_tokens(lexeme);
	// identify_string_tokens(lexeme);
	
	return (token);
}

static char  *clean_instruction(char *instruction)
{
	char *clean;
	char *ptr;
	int i;

	clean = malloc(sizeof(char) * ft_strlen(instruction) + 1);
	ptr = instruction;
	i = 0;
	while (*ptr)
	{
		if (*ptr == '<' || *ptr == '>')
		{
			if(*(ptr + 1) == ' ')
				ptr++;
		}
		clean[i] = *ptr;
		i++;
		ptr++;
	}
	clean[i] = '\0';
	free(instruction);
	return (clean);
}

void tokenizer(t_macro *macro)
{
	static char **lexemes;
	static t_token *tokens;
	static t_token *token;
	static int i;

	macro->instruction = clean_instruction(macro->instruction);
	lexemes = ft_split(macro->instruction, ' ');
	while(lexemes[i])
	{
		token = identify_lexeme(lexemes[i]);
		if (token)
			ft_lstadd_back(&tokens, token);
		i++;
	}
	
}
