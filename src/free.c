/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 13:02:09 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/28 09:30:11 by pmarkaid         ###   ########.fr       */
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

void free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	tmp = cmds;
	while (cmds)
	{
		free_tokens(&tmp->cmd_arg);
		free_tokens(&tmp->redir);
		free(tmp);
		tmp = cmds->next;
	}
}

}

void free_ins(t_macro *macro)
{
	//free_tokens
	//free_cmds
	//free_macro
	free_tokens(macro->tokens);
	
}

void free_all(t_macro *macro)
{
	//free_ins
	//free_macro
}
