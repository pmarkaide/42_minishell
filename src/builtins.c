/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 14:24:52 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/09 14:26:51 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//si gestionamos g_state, esta funcion deberia retornar un int

void    ft_pwd(void) 
{
    char *path;

    path = getcwd(NULL, 0);
    if (path == NULL)
    {
        perror("Error getting current directory");
    }
    else
    {
        ft_putendl_fd(path, STDOUT_FILENO);
        free(path);
    }
    return ;
}

char    *char_pwd(void) 
{
    char *path;

    path = getcwd(NULL, 0);
    if (path == NULL)
    {
        perror("Error getting current directory");
    }
    return (path);
}

void    ft_echo(char *line)
{
    int i;

    i = 4;
    while (line[i] == ' ')
        i++;
    while (line[i])
    {
        ft_putchar_fd(line[i], STDOUT_FILENO);
        i++;
    }
    ft_putchar_fd('\n', STDOUT_FILENO);
}

void    ft_echo_n(char *line)
{
    int i;

    i = 7;
    while (line[i] == ' ')
        i++;
    while (line[i])
    {
        ft_putchar_fd(line[i], STDOUT_FILENO);
        i++;
    }
}

void    ft_env(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        ft_putendl_fd(env[i], STDOUT_FILENO);
        i++;
    }
}

void    ft_exit(t_macro *macro)
{
    free(macro); //FREE GENERAL y muchas más cosas a tener en cuenta: posición del exit, etc
    printf("exit\n");
    //ir al padre y hacer un waitpid
    //Dejar exit para el final 
    exit(0);
}

void ft_export(t_macro *macro)
{
    char    *var;
    char    *name;
    char    *value;
    char    *equal_sign;

    var = macro->instruction + 7;
    while (*var == ' ')
        var++;
    equal_sign = ft_strchr(var, '=');
    if (equal_sign == NULL)
    {
        ft_putendl_fd("export: not a valid identifier", STDERR_FILENO);
        return;
    }
    *equal_sign = '\0';
    name = var;
    value = equal_sign + 1;
    if (setenv(name, value, 1) != 0)
    {
        //lanzar error
        ft_putendl_fd("export: error setting environment variable", STDERR_FILENO);
    }
    ft_export_do(macro, name, value);
}

void    ft_cd(char *line)
{
    char *path;
    int ret;

    path = line + 3;
    while (*path == ' ')
        path++;
    ret = chdir(path);
    if (ret == -1)
    {
        perror("Error changing directory");
    }
    //printf("**Esto no es output**\nCheck de directorio actual: %s\n", char_pwd());
}

void	ft_unset(t_macro *macro)
{
	char	*var;
	int		ij[2];

	ij[0] = 0;
	var = macro->instruction + 6;
    var = ft_strjoin(var, "=0", 0);
    if (var_in_env(var, macro->env, ij))
        ft_replace_matrix_row(&macro->env, NULL, ij[1]);
}
