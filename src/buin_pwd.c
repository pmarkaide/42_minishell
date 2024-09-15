/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:00:43 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/14 20:27:23 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
