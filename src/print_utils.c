/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 13:02:41 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/05 15:38:23 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*enum_to_char(t_type type)
{
	if (type == STRING)
		return ("STRING");
	else if (type == CMD)
		return ("CMD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == INRED)
		return ("INRED");
	else if (type == OUTRED)
		return ("OUTRED");
	else if (type == HERE_DOC)
		return ("HERE_DOC");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == ARG)
		return ("ARG");
	else if (type == BUILTIN)
		return ("BUILTIN");
	else
		return ("UNKNOWN");
	return (NULL);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("%s: %s\n", enum_to_char(tokens->type), tokens->value);
		tokens = tokens->next;
	}
}
