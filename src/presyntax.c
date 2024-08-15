/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   presyntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:12:06 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/15 16:52:13 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	invalid_char_check(char *instruction)
{
	char	c;
	char	*ptr;

	c = 0;
	ptr = instruction;
	while (*ptr)
	{
		if (*ptr == ';' || *ptr == '&' || (*ptr == '|' && *(ptr + 1) == '|'))
			c = *ptr;
		if (*ptr == '>' || *ptr == '<')
		{
			if (*(ptr + 1) == '>' || *(ptr + 1) == '<')
				ptr++;
			while (*(ptr + 1) == ' ')
				ptr++;
			if (*(ptr + 1) == '\0')
				return ('\n');
			if (!isalnum(*(ptr + 1)) && *(ptr + 1) != '|')
				c = *(ptr + 1);
		}
		ptr++;
	}
	return (c);
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
		if (*ptr == '\'' && in_single_quote == 0)
			in_single_quote = 1;
		else if (*ptr == '\'' && in_single_quote == 1)
			in_single_quote = 0;
		if (*ptr == '"' && in_double_quote == 0)
			in_double_quote = 1;
		else if (*ptr == '"' && in_double_quote == 1)
			in_double_quote = 0;
		ptr++;
	}
	if (in_single_quote == 1)
		return ('\'');
	if (in_double_quote == 1)
		return ('\"');
	return (0);
}

static int	print_syntax_error(char invalid_char)
{
	ft_putstr_fd("minishell: ", 2);
	if (invalid_char == '|')
		ft_printf("syntax error near unexpected token `|'\n");
	else if (invalid_char == '\n')
		ft_printf("syntax error near unexpected token `newline'\n");
	else
		ft_printf("syntax error near unexpected token `%c'\n", invalid_char);
	return (1);
}

int	syntax_error_check(char *instruction)
{
	char	c;

	c = 0;
	remove_extra_spaces(instruction);
	white_spaces_into_spaces(instruction);
	c = invalid_char_check(instruction);
	if (c != 0)
		return (print_syntax_error(c));
	c = unclosed_quote_check(instruction);
	if (c != 0)
		return (print_syntax_error(c));
	return (0);
}
