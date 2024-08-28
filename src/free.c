/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 13:02:09 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/28 16:07:00 by pmarkaid         ###   ########.fr       */
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

void	free_array(char ***array)
{
	size_t	i;

	if (*array == NULL)
		return ;
	i = 0;
	while ((*array)[i] != NULL)
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}

void	free_string(char **str)
{
	if (str != NULL && *str != NULL)
	{
		free(*str);
		*str = NULL;
	}
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
	free(macro);
}
