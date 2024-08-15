/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bu_func.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:55:31 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/15 09:43:27 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_builtins(t_macro *macro)
{
	char	*line;

	// printf("-------OUTPUT DE MINISHELL (CD IMPRIME DIRECTORIO ACTUAL)----------\n");
	line = macro->instruction;
	if (ft_strncmp(line, "pwd", 3) == 0)
		ft_pwd();
	else if (ft_strncmp(line, "cd", 2) == 0)
		ft_cd(line);
	else if (ft_strncmp(line, "echo -n", 7) == 0)
		ft_echo_n(line);
	else if (ft_strncmp(line, "echo", 4) == 0)
		ft_echo(line);
	else if (ft_strncmp(line, "exit", 4) == 0)
		ft_exit(macro);
	else if (ft_strncmp(line, "env", 3) == 0)
		ft_env(macro->env);
	else if (ft_strncmp(line, "export", 6) == 0)
		ft_export(macro);
	else if (ft_strncmp(line, "unset", 5) == 0)
		ft_unset(macro);
	else
		// ft_putendl_fd("Not builtin", STDOUT_FILENO);
		return ;
}

char *remove_path(char *cmd)
{
	int		pos;
	char	*real_cmd;

	pos = ft_strchr_last(cmd, '/');
	//printf("pos: %d\n", pos);
	if (pos == 0)
		return (cmd);
	real_cmd = ft_substr(cmd, pos + 1, ft_strlen(cmd));
	return (real_cmd);
}


bool	check_builtin(char *real_cmd)
{
	if (ft_strncmp(real_cmd, "echo", 4) == 0)
		return (true);
	if (ft_strncmp(real_cmd, "cd", 2) == 0)
		return (true);
	if (ft_strncmp(real_cmd, "pwd", 3) == 0)
		return (true);
	if (ft_strncmp(real_cmd, "export", 6) == 0)
		return (true);
	if (ft_strncmp(real_cmd, "unset", 5) == 0)
		return (true);
	if (ft_strncmp(real_cmd, "env", 3) == 0)
		return (true);
	if (ft_strncmp(real_cmd, "exit", 4) == 0)
		return (true);
	return (false);
}



static int	ft_pwd2(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("Error getting current directory");
		return (-1);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	return (0);
}

static int ft_echo2(char **args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

static int ft_env2(t_macro *macro)
{
	int	i;

	i = 0;
	while (macro->env[i])
	{
		ft_putendl_fd(macro->env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

static int	ft_exit2(char **args)
{
	(void)args;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(0);
}

static int ft_unset2(char **args, t_macro *macro)
{
	int	i;
	int	j;
	int	len;

	i = 1;
	while (args[i])
	{
		j = 0;
		len = ft_strlen(args[i]);
		while (macro->env[j])
		{
			if (ft_strncmp(args[i], macro->env[j], len) == 0 && macro->env[j][len] == '=')
			{
				free(macro->env[j]);
				while (macro->env[j])
				{
					macro->env[j] = macro->env[j + 1];
					j++;
				}
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}

static int ft_export2(char **args, t_macro *macro)
{
	int	i;
	int	j;
	int	len;
	int argc;

	i = 0;
	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		while (macro->env[i])
		{
			printf("declare -x %s\n", macro->env[i]);
			i++;
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		j = 0;
		len = ft_strlen(args[i]);
		while (macro->env[j])
		{
			if (ft_strncmp(args[i], macro->env[j], len) == 0 && macro->env[j][len] == '=')
			{
				free(macro->env[j]);
				macro->env[j] = ft_strdup(args[i]);
				break ;
			}
			j++;
		}
		if (!macro->env[j])
		{
			macro->env[j] = ft_strdup(args[i]);
			macro->env[j + 1] = NULL;
		}
		i++;
	}
	return (0);
}

int ft_cd2(char **args, t_macro *macro)
{
	char	*path;
	char	*home;
	int		i;
	int		ret;

	i = 0;
	while (macro->env[i])
	{
		if (ft_strncmp(macro->env[i], "HOME=", 5) == 0)
		{
			home = ft_strdup(macro->env[i] + 5);
			break ;
		}
		i++;
	}
	if (!args[1] || ft_strncmp(args[1], "~", 1) == 0)
		path = home;
	else
		path = args[1];
	printf("path: %s\n", path);
	ret = chdir(path);
	printf("ret: %d\n", ret);
	printf("cwd %s\n", getcwd(NULL, 0));
	if (ret == -1)
	{
		perror("Error");
		return (-1);
	}
	
	macro->env = fix_env("OLDPWD", grab_env("PWD", macro->env, 3), macro->env, 6);
	printf("OLDPWD set to %s\n", grab_env("OLDPWD", macro->env, 6));
	macro->env = fix_env("PWD", getcwd(NULL, 0), macro->env, 3);
	printf("PWD set to %s\n", grab_env("PWD", macro->env, 3));
	return (0);
}

int	exec_builtin(char *cmd, char **args, t_macro *macro)
{
	//printf("dentro de exec_builtin cmd ##%s\n", cmd);
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ft_echo2(args));
	if (ft_strncmp(cmd, "cd", 2) == 0)
	{
		printf("dentro de cd\n");
		return (ft_cd2(args, macro));
	}
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (ft_pwd2());
	if (ft_strncmp(cmd, "export", 6) == 0)
		return (ft_export2(args, macro));
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (ft_unset2(args, macro));
	if (ft_strncmp(cmd, "env", 3) == 0)
		return (ft_env2(macro));
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (ft_exit2(args));
	return (0);
}

char	*grab_env(char *var, char **env, int n)
{
	int	i;
	int	m;

	i = 0;
	m = 0;
	while (!ft_strchr(var, '=') && env && env[i])
	{
		m = n;
		if (m < ft_strchr_i(env[i], '='))
			m = ft_strchr_i(env[i], '=');
		if (!ft_strncmp(env[i], var, m))
			return (ft_substr(env[i], m + 1, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}

char	**fix_env(char *var, char *value, char **env, int n)
{
	int		i[2];
	char	*aux[2];

	i[0] = -1;
	aux[0] = ft_strjoin(var, "=", NULL);
	aux[1] = ft_strjoin(aux[0], value, NULL);
	free(aux[0]);
	while (!ft_strchr(var, '=') && env && env[++i[0]])
	{
		i[1] = n;
		if (i[1] < ft_strchr_i(env[i[0]], '='))
			i[1] = ft_strchr_i(env[i[0]], '=');
		if (!ft_strncmp(env[i[0]], var, i[1]))
		{
			aux[0] = env[i[0]];
			env[i[0]] = aux[1];
			free(aux[0]);
			return (env);
		}
	}
	env = ft_add_row(env, aux[1]);
	free(aux[1]);
	return (env);
}