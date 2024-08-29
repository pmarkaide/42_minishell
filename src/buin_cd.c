/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:29 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/29 13:24:43 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd2(char **args, t_macro *macro)
{
	char	*home;
	char	*path;
	char	*oldpwd;
	int		argc;

	home = get_home_directory(macro);
	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	path = parse_arguments(args, macro, home);
	if (path == NULL)
		return (1);
	if (change_directory(path, home) != 0)
		return (1);
	oldpwd = ft_strjoin(macro->m_pwd, "/", NULL);
	update_environment(macro, oldpwd, path);
	free_2_strings(&home, &oldpwd);
	free(path);
	return (0);
}
