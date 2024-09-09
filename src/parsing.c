/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 09:53:20 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/09 11:14:26 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_here_doc_not_needed(t_token *tokens)
{
	t_token	*tmp;
	t_token	*last;
	int		fd;

	last = NULL;
	tmp = tokens;
	while (tmp && is_redir(tmp, "input"))
	{
		last = tmp;
		tmp = tmp->next;
	}
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == HERE_DOC && tmp != last)
		{
			fd = ft_atoi(tmp->value);
			if (fd != -1)
				close(fd);
		}
		tmp = tmp->next;
	}
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
