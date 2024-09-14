/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 09:53:20 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/14 20:13:16 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_parsing_tokens(t_macro *macro, t_cmd **cmds, int *n)
{
	char	c;

	*n = 1;
	*cmds = NULL;
	c = parsing_error_check(macro->tokens, macro);
	if (c != 0)
	{
		free_ins(macro);
		return (-1);
	}
	else
		*cmds = parse_tokens(macro->tokens, n);
	if (!*cmds)
	{
		free_ins(macro);
		return (-1);
	}
	return (0);
}

static int	handle_here_doc(t_cmd *cmds, t_macro *macro)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (process_cmd(cmd, macro) == -1)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}

int	parsing(t_macro *macro)
{
	int		n;
	t_cmd	*cmds;

	if (check_parsing_tokens(macro, &cmds, &n) == -1)
		return (-1);
	macro->num_cmds = n - 1;
	if (handle_here_doc(cmds, macro) == -1)
	{
		free_cmds(&cmds);
		free_ins(macro);
		return (-1);
	}
	macro->cmds = cmds;
	return (0);
}
