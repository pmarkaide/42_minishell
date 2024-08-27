/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/27 09:17:34 by dbejar-s         ###   ########.fr       */
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
                continue;
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

static int	validate_access(char *file)
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

static char	*search_for_executable(t_macro *macro, t_cmd *cmd)
{
	char	*executable;
	char	**paths;
	char	*full_path;

	executable = ft_strdup(cmd->cmd_arg->value);
	if (!executable)
		return (NULL);
	paths = parse_paths(macro->env);
	if (!paths)
	{
		free(executable);
		return (NULL);
	}
	full_path = get_executable_path(paths, executable);
	free(paths);
	free(executable);
	if (!full_path)
		return (NULL);
	else
		return (full_path);
}

int	validate_executable(t_macro *macro, t_cmd *cmd)
{
	char	*full_path;

	if (ft_strchr("./", cmd->cmd_arg->value[0]) == NULL)
	{
		full_path = search_for_executable(macro, cmd);
		if (!full_path)
			return (1);
		else
		{
			free(cmd->cmd_arg->value);
			cmd->cmd_arg->value = full_path;
		}
	}
	g_exit = validate_access(cmd->cmd_arg->value);
	return (g_exit);
}
