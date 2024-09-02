/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:43:00 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/01 13:33:48 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	int		total_len;
	char	*res;

	if (!s1 || !s2 || !s3)
		return (NULL);
	total_len = 0;
	total_len += ft_strlen(s1);
	total_len += ft_strlen(s2);
	total_len += ft_strlen(s3);
	res = (char *)ft_calloc(sizeof(char), (total_len + 1));
	if (!res)
		return (NULL);
	ft_strcpy(res, s1);
	ft_strcat(res, s2);
	ft_strcat(res, s3);
	return (res);
}

t_macro	*start_env(t_macro *macro, char **argv)
{
	char	*num;
	char	*str;

	str = getcwd(NULL, 0);
	macro->env = fix_env("PWD", str, macro->env, 3);
	free(str);
	str = grab_env("SHLVL", macro->env, 5);
	if (!str || ft_atoi(str) <= 0)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(str) + 1);
	free(str);
	macro->env = fix_env("SHLVL", num, macro->env, 5);
	free(num);
	str = grab_env("PATH", macro->env, 4);
	if (!str)
		macro->env = fix_env("PATH",
				"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", macro->env, 4);
	free(str);
	str = grab_env("_", macro->env, 1);
	if (!str)
		macro->env = fix_env("_", argv[0], macro->env, 1);
	free(str);
	return (macro);
}

char	*grab_home(t_macro *macro)
{
	char	*home;

	home = grab_env("HOME", macro->env, 4);
	if (home == NULL)
		home = ft_strdup("/home/");
	return (home);
}

t_macro	*init_macro(char **envp, char **argv)
{
	t_macro	*macro;

	macro = malloc(sizeof(t_macro));
	if (!macro)
	{
		ft_putstr_fd("Error: Malloc failed creating macro structure\n", 2);
		exit(1);
	}
	ft_bzero(macro, sizeof(t_macro));
	macro->envp = envp;
	macro->env = copy_env(envp);
	macro->history = NULL;
	macro->ins = NULL;
	macro->tokens = NULL;
	macro->cmds = NULL;
	macro->pid = NULL;
	macro = start_env(macro, argv);
	macro->m_pwd = char_pwd();
	macro->m_home = grab_home(macro);
	macro->exit_code = 0;
	macro->pipe_fd[0] = -1;
	macro->pipe_fd[1] = -1;
	macro->pipe_exit[0] = -1;
	macro->pipe_exit[1] = -1;
	return (macro);
}
