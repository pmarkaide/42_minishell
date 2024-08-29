/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   presyntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:12:06 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 16:13:32 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	invalid_char_check(char *instruction)
{
	int	i;

	i = 0;
	while (instruction[i])
	{
		if (!is_in_quote(instruction, i))
		{
			if (instruction[i] == ';')
				return (instruction[i]);
			if (instruction[i] == '&')
				return (instruction[i]);
			if (instruction[i] == '|' && instruction[i + 1] == '|')
				return (instruction[i]);
		}
		i++;
	}
	return (0);
}

static char	valid_file_name(char *instruction)
{
	int		i;
	char	next_char;

	i = 0;
	while (instruction[i])
	{
		if (!is_in_quote(instruction, i))
		{
			if (instruction[i] == '>' || instruction[i] == '<')
			{
				if (instruction[i + 1] == '>' || instruction[i + 1] == '<')
					i++;
				while (instruction[i + 1] == ' ')
					i++;
				if (instruction[i + 1] == '\0')
					return ('\n');
				next_char = instruction[i + 1];
				if (!ft_isalnum(next_char) && !strchr("$~+-./\'\"", next_char))
					return (next_char);
			}
		}
		i++;
	}
	return (0);
}

static char	unclosed_quote_check(char *instruction)
{
	int		in_single_quote;
	int		in_double_quote;
	char	*ptr;

	in_single_quote = 0;
	in_double_quote = 0;
	ptr = instruction;
	while (*ptr)
	{
		if (*ptr == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (*ptr == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		ptr++;
	}
	if (in_single_quote)
		return ('\'');
	if (in_double_quote)
		return ('\"');
	return (0);
}

static int	print_syntax_error(char invalid_char)
{
    ft_putstr_fd("minishell: ", 2);
    if (invalid_char == '|')
        ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
    else if (invalid_char == '\n')
        ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
    else
    {
        ft_putstr_fd("syntax error near unexpected token `", 2);
        ft_putchar_fd(invalid_char, 2);
        ft_putstr_fd("'\n", 2);
    }
    return (2);
}

int	syntax_error_check(t_macro *macro, char *instruction)
{
	char	c;

	c = invalid_char_check(instruction);
	if (c != 0)
	{
		macro->exit_code = 2;
		return (print_syntax_error(c));
	}
	c = valid_file_name(instruction);
	if (c != 0)
	{
		macro->exit_code = 2;
		return (print_syntax_error(c));
	}
	c = unclosed_quote_check(instruction);
	if (c != 0)
	{
		macro->exit_code = 2;
		return (print_syntax_error(c));
	}
	macro->instruction = instruction;
	return (0);
}
