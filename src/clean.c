/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:39:10 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 09:42:19 by dbejar-s         ###   ########.fr       */
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

static char	*clean_instruction(char *instruction)
{
	char	*clean;
	char	*ptr;
	int		i;

	//clean = malloc(sizeof(char) * (ft_strlen(instruction) + 1));
	clean = ft_calloc(ft_strlen(instruction) + 1, sizeof(char));
	ptr = instruction;
	i = 0;
	while (*ptr)
	{
		// Riesgo de Segfault si da la casualidad de que ptr[0] es espacio e intenta leer ptr[-1]
		if (*ptr == ' ' && (*(ptr - 1) == '<' || *(ptr - 1) == '>'))
			ptr++;
		clean[i] = *ptr;
		i++;
		ptr++;
	}
	clean[i] = '\0';
	free(instruction);
	return (clean);
}

int	expand_variable(char *clean, int j, char *instruction, int i,
		t_macro *macro)
{
	char	*envir_name;
	char	*envir_value;

	envir_name = get_envir_name(&instruction[i]);
	//printf("envir_name: %s\n", envir_name);
	envir_value = get_envir_value(envir_name, macro);
	if (envir_name && envir_value)
	{
		ft_strlcpy(&clean[j], envir_value, ft_strlen(envir_value) + 1);
		j += ft_strlen(envir_value);
		i += ft_strlen(envir_name);
		// free(envir_value);
		// free(envir_name);
	}
	if (!envir_value)
	{
		//printf("envir_value DENTRO: %s\n", envir_value);
		i += ft_strlen(envir_name);
	}
	free(envir_value);
	free(envir_name);
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
		if(instruction[i] == '$' && (instruction[i + 1] == '\0' || ft_isdelim(instruction[i + 1])))
			clean[j++] = instruction[i++];
		else if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			//printf("llegas aqui\n");
			i++;
			if (instruction[i] == '?')
			{
				ft_strlcpy(&clean[j], ft_itoa(g_exit), ft_strlen(ft_itoa(g_exit)) + 1);
				j += ft_strlen(ft_itoa(g_exit));
				i++;
			}
			else
			{
			//printf("i antes %d y clean %s\n", i, clean);
			i = expand_variable(clean, j, instruction, i, macro);
			//printf("i despues%d y clean %s\n", i, clean);
			j += ft_strlen(&clean[j]);
			//printf("j: %d\n", j);
			}
		}
		else
		{
			clean[j++] = instruction[i++];
			//printf("hay mas clean %s\n", clean);
		}
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
		if (instruction[i] == '$' && (instruction[i + 1] == '\0' || ft_isdelim(instruction[i + 1])))
			i++;
		else if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			i++;
			if(instruction[i] == '?')
				envir_len += ft_strlen(ft_itoa(g_exit));
			else
			{
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
	//printf("envir_len: %zu\n", total_len);
	clean = (char *)ft_calloc(1, sizeof(char) * total_len + 1); //ft_calloc ya protege de NULL
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
	char	*clean_token;
	t_list	*new_node;

	//clean_token = create_and_clean_token(start, length);
	clean_token = ft_strndup(start, length);
	if (!clean_token)
		return (-1);
	new_node = ft_lstnew(clean_token);
	if (!new_node)
	{
		free(clean_token);
		return (-1);
	}
	ft_lstadd_back(tokens, new_node);
	return (0);
}

static int	ft_2quote(char c)
{
	return (c == '\"');
}
static int	ft_1quote(char c)
{
	return (c == '\'');
}
/* DEJO AQUI LA FUNCION ANTIGUA PARA PODER COMPARARLA CON LA NUEVA
static int	process_token(char **current_pos, t_list **tokens)
{
	char	*token_start;
	char	quote_char;

	token_start = *current_pos;
	quote_char = '\0';
	while (**current_pos && (quote_char || !ft_isdelim(**current_pos)))
	{
		if (ft_isquote(**current_pos))
		{
			if (quote_char == '\0')
				quote_char = **current_pos;
			else if (quote_char == **current_pos)
				quote_char = '\0';
		}
		(*current_pos)++;
	}
	if (*current_pos != token_start)
	{
		if (create_and_add_node(token_start, *current_pos - token_start,
				tokens) == -1)
			return (-1);
	}
	return (0);
}*/

static int	process_token(char **current_pos, t_list **tokens)
{
	bool	double_quote_open;
	bool	single_quote_open;
	int len;
	char *res;
	
	len = 0;
	res = (char *)ft_calloc((ft_strlen(*current_pos) + 1), sizeof(char));
	double_quote_open = false;
	single_quote_open = false;
	while (**current_pos && !ft_isdelim(**current_pos))
	{
		if (ft_2quote(**current_pos)) //empaquetar este if en otra función y llamarla desde aquí pasando punteros
		{
			if (ft_2quote(*(*current_pos + 1)))
				(*current_pos) = (*current_pos) + 2;
			else
			{
				double_quote_open = !double_quote_open;
				(*current_pos)++;
				while (double_quote_open && **current_pos)
				{
					res[len++] = **current_pos;
					(*current_pos)++;
					if (ft_2quote(**current_pos))
					{
						double_quote_open = !double_quote_open;
						(*current_pos)++;
					}
				}
			}
		}
		else if (ft_1quote(**current_pos)) //empaquetar este if en otra función y llamarla desde aquí pasando punteros
		{
			if (ft_1quote(*(*current_pos + 1)))
				(*current_pos) = (*current_pos) + 2;
			else
			{
				single_quote_open = !single_quote_open;
				(*current_pos)++;
				while(single_quote_open && **current_pos)
				{
					res[len++] = **current_pos;
					(*current_pos)++;
					if (ft_1quote(**current_pos))
					{
						single_quote_open = !single_quote_open;
						(*current_pos)++;
					}
				}
			}
		}
		else
		{
		res[len++] = **current_pos;
		(*current_pos)++;
		}
	}
	res[len] = '\0';
	if (len > 0)
	{
		if (create_and_add_node(res, len, tokens) == -1)
			return (1);
	}
	return (0);
}

t_list	*split_args_by_quotes(char *input)
{
	char	*current_pos;
	t_list	*tokens;

	current_pos = input;
	tokens = NULL;
	while (*current_pos)
	{
		while (*current_pos && ft_isdelim(*current_pos))
			current_pos++;
		if (*current_pos)
		{
			if (process_token(&current_pos, &tokens) == -1)
			{
				ft_lstclear(&tokens, free);
				return (NULL);
			}
		}
	}
	return (tokens);
}

void	clean(t_macro *macro)
{
	//printf("antes expanded instruction: %s\n", macro->instruction);
	macro->instruction = get_expanded_instruction(macro->instruction, macro);
	//printf("expanded instruction: %s\n", macro->instruction);
	macro->instruction = clean_instruction(macro->instruction);
	//printf("clean instruction: %s\n", macro->instruction);
}
