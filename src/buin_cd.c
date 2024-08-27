/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:29 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/27 11:50:24 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

int	ft_cd2(char **args, t_macro *macro)
{
	char	*path;
	char	*home;
	int		i;
	int		argc;
	int		home_flag;
	char	*oldpwd;

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
	if (home != NULL && (!args[1] || ft_strncmp(args[1], "~", 1) == 0))
		path = home;
	else
		path = args[1];
	if (access(path, X_OK) != 0)
	{
		perror("Error: Cannot change directory");
		return (1);
	}
	if (path == NULL)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (1);
	}
	if (ft_strncmp(path, "-", 1) == 0)
	{
		path = grab_env("OLDPWD", macro->env, 6);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: XXXcd: ", STDERR_FILENO);
		perror(path);
		return (1);
	}
	macro->env = fix_env("OLDPWD", grab_env("PWD", macro->env, 3), macro->env,
			6);
	oldpwd = ft_calloc(sizeof(char *), ft_strlen(grab_env("PWD", macro->env, 3)));
	oldpwd = grab_env("PWD", macro->env, 3);
	oldpwd = ft_strjoin(oldpwd, "/", NULL);
	if (getcwd(NULL, 0) == NULL)
	{
		macro->env = fix_env("PWD", ft_strjoin(oldpwd, args[1], NULL), macro->env, 3);
		return (0);
	}
	macro->env = fix_env("PWD", getcwd(NULL, 0), macro->env, 3);
	return (0);
}
