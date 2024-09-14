/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:00:43 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/14 20:26:51 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_path(char *cmd)
{
	char	*pos;
	char	*real_cmd;

	pos = ft_strrchr(cmd, '/');
	if (pos == NULL)
		return (cmd);
	real_cmd = ft_substr(pos + 1, 0, ft_strlen(pos + 1));
	return (real_cmd);
}

int	ft_pwd(t_macro *macro)
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
