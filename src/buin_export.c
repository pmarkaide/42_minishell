/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:44 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/27 11:48:40 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

static void	export_argless(t_macro *macro)
{
	int		i;
	char	*var;
	char	*value;

	i = 0;
	while (macro->env[i])
	{
		if (ft_strchr(macro->env[i], '='))
		{
			var = ft_substr(macro->env[i], 0, ft_strchr_i(macro->env[i], '='));
			value = ft_substr(macro->env[i], ft_strchr_i(macro->env[i], '=')
					+ 1, ft_strlen(macro->env[i]));
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(var, STDOUT_FILENO);
			if (value)
			{
				ft_putstr_fd("=\"", STDOUT_FILENO);
				ft_putstr_fd(value, STDOUT_FILENO);
				ft_putstr_fd("\"\n", STDOUT_FILENO);
			}
			else
				ft_putchar_fd('\n', STDOUT_FILENO);
			free(var);
			free(value);
		}
		else
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(macro->env[i], STDOUT_FILENO);
		}
		i++;
	}
}

static int	check_export(char *arg)
{
	int	i;
	int	len;

	i = 0;
	if (arg[i] != '_' && !ft_isalpha(arg[i]))
		return (0);
	len = ft_strchr_i(arg, '=');
	while (arg[i] && arg[i] != '=' && (i < len || len == -1))
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*clean;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	clean = malloc(sizeof(char) * len + 1);
	if (!clean)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			clean[j] = str[i];
			j++;
		}
		i++;
	}
	clean[j] = '\0';
	return (clean);
}

int	ft_export2(char **args, t_macro *macro)
{
	int		i;
	int		j;
	int		len_var;
	int		argc;
	char	*clean_value;
	int		exit_flag;

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
		if (check_export(args[i]) == 0)
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(remove_quotes(args[i]), STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			i++;
			exit_flag = 1;
			continue ;
		}
		clean_value = remove_quotes(args[i]);
		j = 0;
		while (macro->env[j])
		{
			len_var = ft_strchr_i(macro->env[j], '=');
			if (ft_strncmp(clean_value, macro->env[j], len_var) == 0)
			{
				free(macro->env[j]);
				macro->env[j] = ft_strdup(clean_value);
				break ;
			}
			j++;
		}
		if (!macro->env[j])
		{
			macro->env[j] = ft_strdup(clean_value);
			macro->env[j + 1] = NULL;
		}
		free(clean_value);
		i++;
	}
	if (exit_flag == 1)
		return (1);
	return (0);
}
