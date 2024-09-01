/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   presyntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:12:06 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/01 13:35:47 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	invalid_char_check(char *ins)
{
	int	i;

	i = 0;
	while (ins[i])
	{
		if (!is_in_quote(ins, i))
		{
			if (ins[i] == ';')
				return (ins[i]);
			if (ins[i] == '&')
				return (ins[i]);
			if (ins[i] == '|' && ins[i + 1] == '|')
				return (ins[i]);
		}
		i++;
	}
	return (0);
}

static char	valid_file_name(char *ins)
{
	int		i;
	char	next_char;

	i = 0;
	while (ins[i])
	{
		if (!is_in_quote(ins, i))
		{
			if (ins[i] == '>' || ins[i] == '<')
			{
				if (ins[i + 1] == '>' || ins[i + 1] == '<')
					i++;
				while (ins[i + 1] == ' ')
					i++;
				if (ins[i + 1] == '\0')
					return ('\n');
				next_char = ins[i + 1];
				if (!ft_isalnum(next_char) && !ft_strchr("$~+-./\'\"",
						next_char))
					return (next_char);
			}
		}
		i++;
	}
	return (0);
}

static char	unclosed_quote_check(char *ins)
{
	int		in_single_quote;
	int		in_double_quote;
	char	*ptr;

	in_single_quote = 0;
	in_double_quote = 0;
	ptr = ins;
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

static void	print_syntax_error(t_macro *macro, char invalid_char)
{
	macro->exit_code = 2;
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
}

int	syntax_error_check(t_macro *macro, char *ins)
{
	char	c;

	c = invalid_char_check(ins);
	if (c == 0)
	{
		c = valid_file_name(ins);
		if (c == 0)
		{
			c = unclosed_quote_check(ins);
			if (c == 0)
			{
				macro->ins = ins;
				return (0);
			}
		}
	}
	print_syntax_error(macro, c);
	return (-1);
}
