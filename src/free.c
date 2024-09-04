/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 13:02:09 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/04 11:05:34 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*free_tokens(t_token **tokens)
{
	t_token	*tmp;

	while (*tokens)
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free_string(&tmp->value);
		free(tmp);
	}
	*tokens = NULL;
	return (NULL);
}

t_cmd	*free_cmds(t_cmd **cmds)
{
	t_cmd	*tmp;
	t_cmd	*next;

	if (cmds == NULL || *cmds == NULL)
		return (NULL);
	tmp = *cmds;
	while (tmp != NULL)
	{
		next = tmp->next;
		free_tokens(&tmp->cmd_arg);
		free_tokens(&tmp->redir);
		free(tmp);
		tmp = next;
	}
	*cmds = NULL;
	return (NULL);
}

t_macro	*free_ins(t_macro *macro)
{
	free_tokens(&macro->tokens);
	free_cmds(&macro->cmds);
	free(macro->pid);
	close_fds(macro);
	macro->num_cmds = 0;
	return (NULL);
}

t_macro	*free_macro(t_macro *macro)
{
	free_ins(macro);
	free_array(&macro->env);
	free_string(&macro->ins);
	free_string(&macro->m_pwd);
	free_string(&macro->m_home);
	free(macro);
	return (NULL);
}
