/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:39:10 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 10:35:46 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

/*
 * Transverse the original instruction
 * If envir surrounded by S quotes, continue
 * If envir surrounded by double quotes, expand, maintaining the quotes
 * If envirs outside quotes, expand without quotes
 * Tokenize the new instruction
 * Continue parsing, execution etc.
 */

void	fix_redirections(char *instruction)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (instruction[i] != '\0')
	{
		if (instruction[i] == '>' || instruction[i] == '<')
		{
			instruction[j++] = instruction[i];
			i++;
			while (ft_isspace(instruction[i]))
				i++;
			i--;
		}
		else
			instruction[j++] = instruction[i];
		i++;
	}
	instruction[j] = '\0';
}

int	expand_variable(char *clean, int j, char *instruction, int i,
		t_macro *macro)
{
	char	*envir_name;
	char	*envir_value;

	envir_name = get_envir_name(&instruction[i]);
	envir_value = get_envir_value(envir_name, macro);
	if (envir_name && envir_value)
	{
		ft_strlcpy(&clean[j], envir_value, ft_strlen(envir_value) + 1);
		j += ft_strlen(envir_value);
		i += ft_strlen(envir_name);
		free(envir_value);
		free(envir_name);
	}
	return (i);
}

char	*build_expanded_instruction(char *clean, char *instruction,
		t_macro *macro)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (instruction[i])
	{
		if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			i++;
			i = expand_variable(clean, j, instruction, i, macro);
			j += ft_strlen(&clean[j]);
		}
		else
			clean[j++] = instruction[i++];
	}
	clean[j] = '\0';
	return (clean);
}

size_t	expanded_envir_len(char *instruction, t_macro *macro)
{
	size_t	envir_len;
	int		i;
	char	*envir_name;
	char	*envir_value;

	envir_len = 0;
	i = 0;
	while (instruction[i])
	{
		if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			i++;
			envir_name = get_envir_name(&instruction[i]);
			if (envir_name)
			{
				envir_value = get_envir_value(envir_name, macro);
				if (envir_value)
					envir_len += ft_strlen(envir_value);
				i += ft_strlen(envir_name);
				free(envir_name);
			}
		}
		else
			i++;
	}
	return (envir_len);
}

static char	*get_expanded_instruction(char *instruction, t_macro *macro)
{
	char	*clean;
	size_t	envir_len;
	size_t	total_len;

	envir_len = expanded_envir_len(instruction, macro);
	total_len = envir_len + ft_strlen(instruction);
	clean = calloc(1, sizeof(char) * total_len + 1);
	if (!clean)
		return (NULL);
	clean = build_expanded_instruction(clean, instruction, macro);
	clean[total_len] = '\0';
	free(instruction);
	return (clean);
}

// static char	*create_and_clean_token(char *start, size_t length)
// {
// 	char	*token;
// 	char	*clean_token;

// 	token = ft_strndup(start, length);
// 	if (!token)
// 		return (NULL);
// 	clean_token = ft_remove_edge_quotes(token);
// 	free(token);
// 	if (!clean_token)
// 		return (NULL);
// 	token = ft_remove_char(clean_token, '\"');
// 	free(clean_token);
// 	return (token);
// }

static int	create_and_add_node(char *start, size_t length, t_list **tokens)
{
	char	*token;
	t_list	*new_node;

	token = ft_strndup(start, length);
	if (!token)
		return (-1);
	new_node = ft_lstnew(token);
	if (!new_node)
	{
		free(token);
		return (-1);
	}
	ft_lstadd_back(tokens, new_node);
	return (0);
}

static void	process_quote(char *ins, int *pos, int *len)
{
	char	quote_char;

	quote_char = ins[*pos];
	(*pos)++;
	(*len)++;
	while (ins[*pos] && ins[*pos] != quote_char)
	{
		(*len)++;
		(*pos)++;
	}
	if (ins[*pos] == quote_char)
	{
		(*len)++;
		(*pos)++;
	}
}

static int	process_pipe(char *ins, int *pos, t_list **tokens, char **start,
		int *len)
{
	if (*len > 0)
	{
		if (create_and_add_node(*start, *len, tokens) == -1)
			return (-1);
	}
	if (create_and_add_node("|", 1, tokens) == -1)
		return (-1);
	(*pos)++;
	*start = &ins[*pos];
	*len = 0;
	return (0);
}

static void	process_character(int *pos, int *len)
{
	(*len)++;
	(*pos)++;
}

static int	process_token(char *ins, int *pos, t_list **tokens)
{
	char	*start;
	int		len;
	int		result;

	start = &ins[*pos];
	len = 0;
	result = 0;
	while (ins[*pos] && !ft_isdelim(ins[*pos]))
	{
		if (ft_isquote(ins[*pos]))
			process_quote(ins, pos, &len);
		else if (ins[*pos] == '|')
			result = process_pipe(ins, pos, tokens, &start, &len);
		else
			process_character(pos, &len);
		if (result == -1)
			return (-1);
	}
	if (len > 0)
	{
		if (create_and_add_node(start, len, tokens) == -1)
			return (-1);
	}
	return (0);
}

t_list	*split_args_by_quotes(char *ins)
{
	t_list	*tokens;
	int		pos;

	tokens = NULL;
	pos = 0;
	while (ins[pos])
	{
		while (ins[pos] && ft_isdelim(ins[pos]))
			pos++;
		if (ins[pos])
		{
			if (process_token(ins, &pos, &tokens) == -1)
			{
				ft_lstclear(&tokens, ft_del);
				return (NULL);
			}
		}
	}
	return (tokens);
}

void	clean(t_macro *macro)
{
	fix_redirections(macro->instruction);
	macro->instruction = get_expanded_instruction(macro->instruction, macro);
}

