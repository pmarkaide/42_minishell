/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:44 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/03 22:26:18 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_env(char *clean_value, t_macro *macro, int j)
{
	int	value_flag;

	value_flag = ft_strchr_i(clean_value, '=');
	if (value_flag != -1)
	{
		free(macro->env[j]);
		macro->env[j] = ft_strdup(clean_value);
	}
}

static void	add_env(char *clean_value, t_macro *macro)
{
	int		j;
	char	**new_env;
	int		k;

	j = 0;
	while (macro->env[j])
		j++;
	new_env = malloc((j + 2) * sizeof(char *));
	if (new_env == NULL)
	{
		perror("malloc");
		free_macro(macro);
		exit(1);
	}
	k = 0;
	while (k < j)
	{
		new_env[k] = macro->env[k];
		k++;
	}
	new_env[j] = ft_strdup(clean_value);
	new_env[j + 1] = NULL;
	free(macro->env);
	macro->env = new_env;
}

static void	update_or_add_env(char *clean_value, t_macro *macro)
{
	int	j;
	int	len_var;
	int	len;

	len = ft_strchr_i(clean_value, '=');
	if (len == -1)
		len = ft_strlen(clean_value);
	j = 0;
	while (macro->env[j])
	{
		len_var = ft_strchr_i(macro->env[j], '=');
		if (len_var == -1)
			len_var = ft_strlen(macro->env[j]);
		if (ft_strncmp(clean_value, macro->env[j], len_var) == 0
			&& len_var == len)
		{
			update_env(clean_value, macro, j);
			return ;
		}
		j++;
	}
	add_env(clean_value, macro);
}

static void	process_argument(char *arg, t_macro *macro, int *exit_flag)
{
	if (validate_and_clean_argument(arg, exit_flag) == -1)
		return ;
	update_or_add_env(arg, macro);
}

int	ft_export2(char **args, t_macro *macro)
{
	int	i;
	int	argc;
	int	exit_flag;

	exit_flag = 0;
	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		export_argless(macro);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		process_argument(args[i], macro, &exit_flag);
		i++;
	}
	return (exit_flag);
}
