/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bu_func.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:55:31 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/08/09 15:55:41 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    test_builtins(t_macro *macro)
{
    char *line;

    //printf("-------OUTPUT DE MINISHELL (CD IMPRIME DIRECTORIO ACTUAL)----------\n");
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
        //ft_putendl_fd("Not builtin", STDOUT_FILENO);
        return ;
   
}