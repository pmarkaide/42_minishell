/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/14 20:21:39 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_here_doc_not_needed(t_token *tokens)
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
				close_fd(&fd);
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

static void	search_executable(t_macro *macro, t_cmd *cmd)
{
	char	**paths;
	char	*full_path;
	char	*exec;

	full_path = NULL;
	exec = cmd->cmd_arg->value;
	paths = parse_paths(macro->env);
	if (!paths)
		exit_error(exec, "No such file or directory", macro, 127);
	full_path = get_executable_path(paths, exec, macro);
	free_array(&paths);
	if (!full_path)
	{
		ft_putstr_fd(exec, 2);
		ft_putstr_fd(": command not found\n", 2);
		free_macro(macro);
		exit(127);
	}
	else
	{
		free_string(&exec);
		cmd->cmd_arg->value = full_path;
	}
}

void	validation(t_macro *macro, t_cmd *cmd)
{
	if (cmd->cmd_arg && cmd->cmd_arg->type == CMD)
	{
		if (ft_strchr("./", cmd->cmd_arg->value[0]) == NULL)
			search_executable(macro, cmd);
		validate_access(cmd->cmd_arg->value, macro);
	}
	if (validate_redirections(cmd->redir, macro) == -1)
	{
		free_macro(macro);
		exit(1);
	}
}
