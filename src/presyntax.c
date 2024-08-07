/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   presyntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:12:06 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/07 21:22:47 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	invalid_char_check(char *instruction)
{
	char	c;
	char	*ptr;

	c = 0;
	ptr = instruction;
	while (*ptr)
	{
		if (*ptr == ';')
			c = ';';
		if (*ptr == '|' && *(ptr + 1) == '|')
			c = '|';
		if (*ptr == '>' || *ptr == '<')
		{
			if (*(ptr + 1) == '>' || *(ptr + 1) == '<')
				ptr++;
			if (*(ptr + 1) == ' ' && !ft_isalnum(*(ptr + 2)))
				c = *(ptr + 2);
			else if (!ft_isalnum(*(ptr + 1)))
				c = *(ptr + 1);
		}
		ptr++;
	}
	return (c);
}

char	unclosed_quote_check(char *instruction)
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
		return ('"');
	return (0);
}

char	*syntax_error_check(char *instruction)
{
	char	c;

	c = 0;
	remove_extra_spaces(instruction);
	white_spaces_into_spaces(instruction);
	c = invalid_char_check(instruction);
	if (c != 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		exit(258);
	}
	c = unclosed_quote_check(instruction);
	if (c != 0)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for matching`",
			2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		exit(258);
	}
	return (instruction);
}
