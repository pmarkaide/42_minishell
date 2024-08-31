/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 13:02:09 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/31 15:13:07 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_tokens(t_token **tokens)
{
	t_token	*tmp;

	while (*tokens)
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->value);
		free(tmp);
	}
	*tokens = NULL;
	return(NULL);
}

char	*free_cmds(t_cmd **cmds)
{
	t_cmd	*tmp;
	t_cmd	*next;

	if (cmds == NULL || *cmds == NULL)
		return(NULL);
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
	return(NULL);
}

char	*free_ins(t_macro *macro)
{
	free_tokens(&macro->tokens);
	free_cmds(&macro->cmds);
	free(macro->pid);
	close_fds(macro, 0);
	macro->num_cmds = 0;
	return(NULL);
}

char	*free_macro(t_macro *macro)
{
	free_ins(macro);
	free_array(&macro->env);
	free_array(&macro->history);
	free_string(&macro->instruction);
	free_string(&macro->m_pwd);
	free_string(&macro->m_home);
	free(macro);
	return(NULL);
}
