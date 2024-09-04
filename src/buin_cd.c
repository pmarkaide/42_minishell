/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:29 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/03 23:08:03 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > 2)
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
	return (i);
}

int	ft_cd2(char **args, t_macro *macro)
{
	char	*home;
	char	*path;
	char	*oldpwd;
	int		argc;

	argc = count_args(args);
	if (argc > 2)
		return (1);
	home = get_home_directory(macro);
	if (home == NULL && (!args[1] || args[1][0] == '\0'))
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (1);
	}
	path = parse_arguments(args, macro, home);
	if (path == NULL)
		return (1);
	if (change_directory(path, home) != 0)
		return (1);
	oldpwd = ft_strjoin(macro->m_pwd, "/", NULL, 0);
	update_environment(macro, oldpwd, path);
	free_2_strings(&home, &oldpwd);
	free(path);
	return (0);
}
