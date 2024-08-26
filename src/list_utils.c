/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:43:43 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/25 15:05:25 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	ft_bzero(token, sizeof(t_token));
	return (token);
}

void	token_add_back(t_token **tokens, t_token *new)
{
	if (!tokens || !new)
		return ;
	if (!*tokens)
		*tokens = new;
	else
		(last_token(*tokens))->next = new;
}

void remove_token(t_token **tokens, t_token *token)
{
    t_token *current = *tokens;
    t_token *prev = NULL;

    while (current != NULL)
    {
        if (current == token)
        {
            if (prev != NULL)
                prev->next = current->next;
            else
                *tokens = current->next;
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

t_token	*last_token(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	return (cmd);
}

void	cmd_add_back(t_cmd **cmds, t_cmd *new)
{
	if (!cmds || !new)
		return ;
	if (!*cmds)
		*cmds = new;
	else
		(last_cmd(*cmds))->next = new;
}

t_cmd	*last_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}

bool	is_last_of_type(t_token *tokens, t_type type)
{
	t_token	*tmp;

	tmp = tokens->next;
	while (tmp)
	{
		if (tmp->type == type)
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

int	tokens_size(t_token *tokens)
{
	size_t	i;

	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	return ((int)i);
}
