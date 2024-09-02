/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:44 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/02 13:30:27 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_invalid_identifier(char *arg, int *exit_flag)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(remove_quotes(arg), STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	*exit_flag = 1;
}

static void	update_env(char *clean_value, t_macro *macro, int j)
{
	int	value_flag;
	
	value_flag = ft_strchr_i(clean_value, '=');
	if (value_flag != -1)
	{
		free(macro->env[j]);
		macro->env[j] = ft_strdup(clean_value);
		free(clean_value);
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
	free_string(&clean_value);
}

static char *validate_and_clean_argument(char *arg, int *exit_flag)
{
    char *clean_value;

    if (check_export(arg) == 0)
    {
        handle_invalid_identifier(arg, exit_flag);
        return NULL;
    }
    clean_value = remove_quotes(arg);
    return clean_value;
}

static void update_or_add_env(char *clean_value, t_macro *macro)
{
    int j;
    int len_var;
    int len;

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
            return;
        }
        j++;
    }
    add_env(clean_value, macro);
}

static void process_argument(char *arg, t_macro *macro, int *exit_flag)
{
    char *clean_value;

    clean_value = validate_and_clean_argument(arg, exit_flag);
    if (clean_value == NULL)
        return;

    update_or_add_env(clean_value, macro);
}

int	ft_export2(char **args, t_macro *macro)
{
	int		i;
	int		argc;
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
		process_argument(args[i], macro, &exit_flag);
		i++;
	}
	return (exit_flag);
}
