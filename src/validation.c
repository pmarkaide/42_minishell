/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/15 13:32:58 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_access(char *file)
{
	if (!access(file, F_OK))
	{
		if (!access(file, X_OK))
		{
			if (is_directory(file))
				return (error_msg(file, 126));
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
	int		exit_code;

	if (ft_strchr("./", cmd->cmd_arg->value[0]) == NULL)
	{
		full_path = search_for_executable(macro, cmd);
		if (!full_path)
			return (-1);
		else
		{
			free(cmd->cmd_arg->value);
			cmd->cmd_arg->value = full_path;
		}
	}
	exit_code = validate_access(cmd->cmd_arg->value);
	if (exit_code != 0)
		ft_putstr_fd("executable not found\n", 2);
	return (exit_code);
}
