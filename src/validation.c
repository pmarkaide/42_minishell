/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/27 15:04:20 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

int	validate_redirections(t_token *redir)
{
	t_token	*tmp;
	int		fd;

	tmp = redir;
	while (tmp)
	{
		if (is_redir(tmp, "input") || is_redir(tmp, "output"))
		{
			if (tmp->type == HERE_DOC)
			{
				tmp = tmp->next;
				continue ;
			}
			else
			{
				fd = open_file(tmp);
				if (fd == -1)
					return (-1);
				close(fd);
			}
		}
		tmp = tmp->next;
	}
	close_here_doc_not_needed(redir);
	return (0);
}

int	validate_access(char *file)
{
	char	*msg;

	if (!access(file, F_OK))
	{
		if (!access(file, X_OK))
		{
			if (is_directory(file))
			{
				msg = ft_strjoin("minishell: ", file, NULL);
				msg = ft_strjoin(msg, ": Is a directory\n", NULL);
				ft_putstr_fd(msg, 2);
				free(msg);
				return (126);
			}
			return (0);
		}
		return (error_msg(file, 126));
	}
	return (error_msg(file, 127));
}

int	search_executable(t_macro *macro, t_cmd *cmd)
{
	char	**paths;
	char	*full_path;

	full_path = NULL;
	if (ft_strchr("./", cmd->cmd_arg->value[0]) == NULL)
	{
		paths = parse_paths(macro->env);
		if (!paths)
			return (127);
		full_path = get_executable_path(paths, cmd->cmd_arg->value);
		free(paths);
		if (!full_path)
			return (127);
		else
		{
			free(cmd->cmd_arg->value);
			cmd->cmd_arg->value = full_path;
		}
	}
	g_exit = validate_access(cmd->cmd_arg->value);
	return (g_exit);
}
