/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 15:10:56 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_redirections(t_token *redir, t_macro *macro)
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
				fd = open_file(tmp, macro);
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

void	validate_access(char *exec, t_macro *macro)
{
	if (!access(exec, F_OK))
	{
		if (!access(exec, X_OK))
		{
			if (is_directory(exec))
				exit_error(exec, "Is a directory", macro, 126);
			return ;
		}
		exit_error(exec, "Permission denied", macro, 126);
		return ;
	}
	exit_error(exec, "No such file or directory", macro, 127);
}

void	search_executable(t_macro *macro, t_cmd *cmd)
{
	char	**paths;
	char	*full_path;

	full_path = NULL;
	paths = parse_paths(macro->env);
	if (!paths)
		exit_error(cmd->cmd_arg->value, "No such file or directory", macro, 127);
	full_path = get_executable_path(paths, cmd->cmd_arg->value, macro);
	free_array(&paths);
	if (!full_path)
	{
		ft_putstr_fd(cmd->cmd_arg->value, 2);
		ft_putstr_fd(": command not found\n", 2);
		free_ins(macro);
		exit(127);
	}
	else
	{
		free(cmd->cmd_arg->value);
		cmd->cmd_arg->value = full_path;
	}
}

void validation(t_macro *macro, t_cmd *cmd)
{
	
	if (ft_strchr("./", cmd->cmd_arg->value[0]) == NULL)
		search_executable(macro, cmd);
	validate_access(cmd->cmd_arg->value, macro);
	if(validate_redirections(cmd->redir, macro) == -1)
	{
		free_ins(macro);
		exit(1);
	}
}
