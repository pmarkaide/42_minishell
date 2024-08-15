/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:42:50 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/15 23:53:19 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_redir(t_token *token, char *redir_type)
{
    if (!token || !redir_type)
        return false;
    if (ft_strcmp(redir_type, "input") == 0)
	{
        if (token->type == INRED || token->type == HERE_DOC)
            return true;
    } else if (ft_strcmp(redir_type, "output") == 0)
	{
        if (token->type == OUTRED || token->type == APPEND)
            return true;
    }
    return false;
}

bool	is_builtin(t_token *token)
{
	if (ft_strcmp(token->value, "echo") == 0)
		return (true);
	if (ft_strcmp(token->value, "cd") == 0)
		return (true);
	if (ft_strcmp(token->value, "pwd") == 0)
		return (true);
	if (ft_strcmp(token->value, "export") == 0)
		return (true);
	if (ft_strcmp(token->value, "unset") == 0)
		return (true);
	if (ft_strcmp(token->value, "env") == 0)
		return (true);
	if (ft_strcmp(token->value, "exit") == 0)
		return (true);
	return (false);
}


