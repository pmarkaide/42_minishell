/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:39:10 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/20 08:39:55 by dbejar-s         ###   ########.fr       */
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

	clean = malloc(sizeof(char) * ft_strlen(instruction) + 1);
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
		if (instruction[i] == '$' && (instruction[i + 1] == ' ' || instruction[i + 1] == '\0')) //ESTO DEBERIA SER CON  TABULADOR TAMBIEN???
		{
			clean[j++] = instruction[i++];
		}
		else if (instruction[i] == '$' && instruction[i+1] == '?')
		{
			i++;
			ft_strlcpy(&clean[j], ft_itoa(g_exit), ft_strlen(ft_itoa(g_exit)) + 1);
			i += 1;
			j += ft_strlen(ft_itoa(g_exit));
		}
		else if (instruction[i] == '$' && i > 0 && instruction[i - 1] == '\"' && instruction[i+1] == '\"')
		{
			//printf("TE QUEDAS AQUI###############\n");
			clean[j++] = instruction[i++];
		}
		else if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
		{
			//printf("LLEGAS AQUI\n");
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
	// i = 0;
	// //printf("Instruction dentro: %s\n", instruction);
	// while (instruction[i]) //BORRAR ESTE WHILE, SOLO PRUEBAS
	// {
	// 	printf("verdadero o falso: i es%d y char es %c TRUE es 1 y FALSO es O ==>%d\n", i, instruction[i], envir_must_be_expanded(instruction, i));
	// 	i++;
	// }
	i = 0;
	while (instruction[i])
	{
		if (instruction[i] == '$' && (instruction[i + 1] == ' ' || instruction[i + 1] == '\0')) //ESTO DEBERIA SER CON  TABULADOR TAMBIEN???
		{
			i++;
		}
		else if (instruction[i] == '$' && instruction[i + 1] == '?')
		{
			i++;
			envir_len += ft_strlen(ft_itoa(g_exit));
			i += 1; 
		}
		else if (instruction[i] == '$' && envir_must_be_expanded(instruction, i))
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
	//printf("envir_len en funcion: %zu\n", envir_len);
	return (envir_len);
}

static char	*get_expanded_instruction(char *instruction, t_macro *macro)
{
	char	*clean;
	size_t	envir_len;
	size_t	total_len;

	envir_len = expanded_envir_len(instruction, macro);
	//printf("envir_len: %zu\n", envir_len);
	total_len = envir_len + ft_strlen(instruction);
	//printf("total_len: %zu\n", total_len);
	clean = calloc(1, sizeof(char) * total_len + 1);
	if (!clean)
		return (NULL);
	clean = build_expanded_instruction(clean, instruction, macro);
	clean[total_len] = '\0';
	free(instruction);
	return (clean);
}

static char	*create_and_clean_token(char *start, size_t length)
{
	char	*token;
	char	*clean_token;

	token = ft_strndup(start, length);
	if (!token)
		return (NULL);
	clean_token = ft_remove_edge_quotes(token);
	//printf("dentro de funcion create and clean --> clean_token: %s\n", clean_token);
	free(token);
	if (!clean_token)
		return (NULL);
	token = ft_remove_char(clean_token, '\"');
	//printf("dentro de funcion create and clean --> token: %s\n", token);
	free(clean_token);
	return (token);
}

static int	create_and_add_node(char *start, size_t length, t_list **tokens)
{
	char	*clean_token;
	t_list	*new_node;

	clean_token = create_and_clean_token(start, length);
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

static int	process_token(char **current_pos, t_list **tokens) //PROBLEMAS AL SEPARAR COMILLAS EN echo hola'mundo' WIP
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
	macro->instruction = get_expanded_instruction(macro->instruction, macro);
	//printf("Instruction en expanded: %s\n", macro->instruction);
	macro->instruction = clean_instruction(macro->instruction);
	//printf("Instruction en clean: %s\n", macro->instruction);
}
