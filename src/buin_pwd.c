/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:00:43 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/29 22:05:57 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_path(char *cmd)
{
	char	*pos;
	char	*real_cmd;

	pos = ft_strrchr(cmd, '/');
	if (pos == NULL)
		return (cmd);
	real_cmd = ft_substr(pos + 1, 0, ft_strlen(pos + 1));
	return (real_cmd);
}

int	ft_pwd2(t_macro *macro)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		path = ft_strdup(macro->m_pwd);
		if (path == NULL)
		{
			perror("Error getting current directory");
			return (1);
		}
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	return (0);
}

char	*char_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("Error getting current directory");
	}
	return (path);
}
