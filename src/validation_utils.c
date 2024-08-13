/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 23:25:19 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/13 23:26:30 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_directory(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		return (true);
	}
	return (false);
}

char	**parse_paths(char **env)
{
	int		i;
	char	**paths;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			paths = ft_split(env[i] + 5, ':');
			return (paths);
		}
		i++;
	}
	return (NULL);
}

char	*get_executable_path(t_macro *macro, char **paths, char *executable)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], executable, "/");
		if (full_path == NULL)
			return (NULL);
		if (!access(full_path, F_OK))
			return (full_path);
		free(full_path);
		i++;
	}
	ft_putstr_fd("Executable not found\n", 2);
	return (NULL);
}
