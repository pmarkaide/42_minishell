/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:55:31 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/27 11:51:00 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

int	select_and_run_builtin(char *cmd, char **args, t_macro *macro)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ft_echo2(args));
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (ft_cd2(args, macro));
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
