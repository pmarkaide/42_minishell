/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 22:10:13 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/10 15:53:18 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	in_root(char *path)
{
	if (ft_strcmp(path, "/") == 0)
		return (1);
	return (0);
}

static int	in_home(t_macro *macro)
{
	char	*home;

	home = grab_env("HOME", macro->env, 4);
	if (home == NULL)
		return (0);
	if (ft_strcmp(macro->m_pwd, home) == 0)
	{
		free_string(&home);
		return (1);
	}
	free_string(&home);
	return (0);
}

static char	*upper_than_home(t_macro *macro)
{
	char	*home;
	char	*path;
	char	*tmp;
	int		len;

	home = grab_env("HOME", macro->env, 4);
	if (home == NULL)
		return (NULL);
	len = ft_strlen(home);
	if (ft_strncmp(macro->m_pwd, home, len) == 0)
	{
		tmp = ft_strdup(macro->m_pwd);
		path = ft_strjoin3("minishell:~", tmp + len, "$ ");
		free(home);
		free(tmp);
		return (path);
	}
	else
	{
		free(home);
		return (NULL);
	}
}

char	*create_path(t_macro *macro)
{
	char	*path;
	char	*up_home;

	up_home = upper_than_home(macro);
	if (in_root(macro->m_pwd))
		path = ft_strdup("minishell:/$ ");
	else if (in_home(macro))
		path = ft_strdup("minishell:~$ ");
	else if (up_home)
		path = upper_than_home(macro);
	else
		path = ft_strjoin3("minishell:", macro->m_pwd, "$ ");
	free(up_home);
	return (path);
}
