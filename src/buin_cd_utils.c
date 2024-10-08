/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_cd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:15:25 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/10 14:32:25 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_save_env(char *var, t_macro *macro, int size)
{
	char	*tmp;

	tmp = grab_env(var, macro->env, size);
	if (tmp)
		macro->env = fix_env(var, macro->m_pwd, macro->env, size);
	free(tmp);
}

char	*get_home_directory(t_macro *macro)
{
	int	i;

	i = 0;
	while (macro->env[i])
	{
		if (ft_strncmp(macro->env[i], "HOME=", 5) == 0)
			return (ft_strdup(macro->env[i] + 5));
		i++;
	}
	return (NULL);
}

char	*parse_arguments(char **args, t_macro *macro, char *home)
{
	char	*path;

	if (!args[1] || args[1][0] == '\0')
		path = ft_strdup(home);
	else if (ft_strncmp(args[1], "~", 1) == 0 && args[1][1] == '\0')
		path = ft_strdup(macro->m_home);
	else if (ft_strncmp(args[1], "~/", 2) == 0)
		path = ft_strjoin(macro->m_home, args[1] + 1, NULL, 0);
	else if (ft_strncmp(args[1], "-", 1) == 0)
	{
		path = grab_env("OLDPWD", macro->env, 6);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
			free_string(&home);
			return (NULL);
		}
	}
	else
		path = ft_strdup(args[1]);
	return (path);
}

int	change_directory(char *path, char *home)
{
	char	*tmp;

	if (home == NULL && !path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (1);
	}
	if (access(path, X_OK) != 0)
	{
		tmp = ft_strjoin("minishell: cd: ", path, NULL, 2);
		perror(tmp);
		free(tmp);
		free(home);
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
	return (0);
}

void	update_environment(t_macro *macro, char *oldpwd, char *path)
{
	char	*tmp;
	char	*cwd;

	check_save_env("OLDPWD", macro, 6);
	tmp = getcwd(NULL, 0);
	if (tmp == NULL)
	{
		ft_putstr_fd("cd: error retrieving current directory:", STDERR_FILENO);
		ft_putstr_fd(" getcwd: cannot access parent ", STDERR_FILENO);
		ft_putendl_fd("directories: No such file or directory", STDERR_FILENO);
		macro->m_pwd = ft_strjoin(oldpwd, path, NULL, 0);
		check_save_env("PWD", macro, 3);
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("getcwd");
		free(tmp);
		return ;
	}
	free(macro->m_pwd);
	macro->m_pwd = ft_strdup(cwd);
	free_2_strings(&tmp, &cwd);
	check_save_env("PWD", macro, 3);
}
