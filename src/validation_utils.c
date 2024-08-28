/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 23:25:19 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/28 03:13:46 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file(t_token *token, t_macro *macro)
{
	int	fd;

	fd = -1;
	if (token->type == INRED)
		fd = open(token->value, O_RDONLY);
	else if (token->type == OUTRED)
		fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (token->type == APPEND)
		fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == ENOENT)
			macro->exit_code = (NO_FILE);
		else if (errno == EACCES)
			macro->exit_code = (NO_FILE);
		else if (errno == EISDIR)
			macro->exit_code = (NO_FILE);
		//perror(token->value)-> this message is also correct
		exit_error(token->value, strerror(errno), macro, macro->exit_code);
		return (-1);
	}
	return (fd);
}

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
		return(NULL);
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			paths = ft_split(env[i] + 5, ':');
			return (paths);
		}
		i++;
	}
	return(NULL);
}

char	*get_executable_path(char **paths, char *executable, t_macro *macro)
{
	int		i;
	char	*full_path;
	//char	*msg;

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
	macro->exit_code = 127;
	return (NULL);
}
