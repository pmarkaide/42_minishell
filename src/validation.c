/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/27 19:40:24 by pmarkaid         ###   ########.fr       */
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

int	validate_access(char *exec)
{
	char	*msg;

	if (!access(exec, F_OK))
	{
		if (!access(exec, X_OK))
		{
			if (is_directory(exec))
			{
				msg = ft_strjoin("minishell: ", exec, NULL);
				msg = ft_strjoin(msg, ": Is a directory\n", NULL);
				ft_putstr_fd(msg, 2);
				free(msg);
				return (126);
			}
			return (0);
		}
		return (error_msg(exec, 126));
	}
	return (error_msg(exec, 127));
}

int	search_executable(t_macro *macro, t_cmd *cmd)
{
	char	**paths;
	char	*full_path;

	full_path = NULL;
	paths = parse_paths(macro->env);
	if (!paths)
		return(-1);
	full_path = get_executable_path(paths, cmd->cmd_arg->value);
	free(paths);
	if (!full_path)
	{
			ft_putstr_fd(cmd->cmd_arg->value,2);
			ft_putstr_fd(": command not found\n",2);
			exit(g_exit);
	}
	else
	{
		free(cmd->cmd_arg->value);
		cmd->cmd_arg->value = full_path;
	}
	return (g_exit);
}

void validation(t_macro *macro, t_cmd *cmd)
{
	//TODO: create error_exit printer to exit
	if (ft_strchr("./", cmd->cmd_arg->value[0]) == NULL)
		search_executable(macro, cmd);
	g_exit = validate_access(cmd->cmd_arg->value);
	if(g_exit != 0)
	{
		error_msg(cmd->cmd_arg->value, g_exit);
		exit(g_exit);
	}
	g_exit = validate_redirections(cmd->redir);
	if(g_exit != 0)
	{
		error_msg(cmd->cmd_arg->value, g_exit);
		exit(g_exit);
	}
}
