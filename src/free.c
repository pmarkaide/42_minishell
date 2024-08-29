/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 13:02:09 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 13:23:35 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **tokens)
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
}

void	free_cmds(t_cmd **cmds)
{
	t_cmd	*tmp;
	t_cmd	*next;

	if (cmds == NULL || *cmds == NULL)
		return ;
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
}

void	free_ins(t_macro *macro)
{
	free_tokens(&macro->tokens);
	free_cmds(&macro->cmds);
	free(macro->pid);
	close_fds(macro->pipe_fd, 0);
	macro->num_cmds = 0;
}

void	free_macro(t_macro *macro)
{
	free_array(&macro->env);
	free_array(&macro->history);
	free_string(&macro->instruction);
	free_ins(macro);
	free_string(&macro->m_pwd);
	free_string(&macro->m_home);
	close_fds(macro->pipe_fd, 0);
	free(macro);
}
