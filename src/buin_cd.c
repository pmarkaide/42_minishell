/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:29 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/28 18:19:59 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd2(char **args, t_macro *macro)
{
	char	*path;
	char	*home;
	int		i;
	int		argc;
	int		home_flag;
	char	*oldpwd;
	char	*tmp;

	i = 0;
	home_flag = 0;
	while (macro->env[i])
	{
		if (ft_strncmp(macro->env[i], "HOME=", 5) == 0)
		{
			home = ft_strdup(macro->env[i] + 5);
			home_flag = 1;
			break ;
		}
		i++;
	}
	if (home_flag == 0)
		home = NULL;
	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (!args[1] || args[1][0] == '\0')
		path = home;
	else if (ft_strncmp(args[1], "~", 1) == 0)
		path = ft_strdup(macro->m_home);
	else if (ft_strncmp(args[1], "-", 1) == 0)
	{
		path = grab_env("OLDPWD", macro->env, 6);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
			free(home);
			return (1);
		}
	}
	else
		path = ft_strdup(args[1]);
	if (home == NULL && !path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (1);
	}
	if (access(path, X_OK) != 0)
	{
		path = ft_strjoin("minishell: cd: ", path, NULL);
		perror(path);
		free(home);
		free(path);
		return (1);
	}
	if (chdir(path) == -1)
	{	
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(path);
		free(home);
		free(path);
		return (1);
	}
	tmp = grab_env("OLDPWD", macro->env, 6);
	if (tmp)
		macro->env = fix_env("OLDPWD", macro->m_pwd, macro->env, 6);
	//oldpwd = ft_calloc(sizeof(char *), ft_strlen(macro->m_pwd) + 2);
	oldpwd = ft_strjoin(macro->m_pwd, "/", NULL);
	free(tmp);
	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
	{
		ft_putendl_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory", STDERR_FILENO);
		macro->m_pwd = ft_strjoin(oldpwd, args[1], NULL);
		if (grab_env("PWD", macro->env, 3))
			macro->env = fix_env("PWD", macro->m_pwd, macro->env, 3);
		free(home);
		free(oldpwd);
		free(path);
		return (0);
	}
	macro->m_pwd = getcwd(NULL, 0);
	free(tmp);
	tmp = grab_env("PWD", macro->env, 3);
	if (tmp)
		macro->env = fix_env("PWD", macro->m_pwd, macro->env, 3);
	free(tmp);
	free(home);
	free(oldpwd);
	free(path);
	return (0);
}
