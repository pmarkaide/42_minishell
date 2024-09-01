/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:49:38 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/01 13:33:48 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit;

int main(int argc, char **argv, char **envp)
{
    t_macro *macro;
    char *line;
    char *path;

    g_exit = 0;
    macro = init_macro(envp, argv);
    signal(SIGINT, ft_signal_handler);
    signal(SIGQUIT, SIG_IGN);

    if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        // Handle command passed via -c option
        line = argv[2];

        if (line && !ft_str_empty(line))
        {
            macro->ins = line;
            tokenizer(macro);
            parsing(macro);
            if (macro->cmds)
            {
                execution(macro);
                free_ins(macro);
            }
        }

        free_macro(macro);
        exit(0);
    }

    while (1)
    {
        if (isatty(fileno(stdin)))
        {
            if (g_exit == 130)
                line = readline("");
            else
            { 
                path = create_path(macro);
                line = readline(path);
                free(path);
            }
        }
        else
        {
            line = get_next_line(fileno(stdin));
            if (line)
            {
                char *trimmed_line = ft_strtrim(line, "\n");
                free(line);
                line = trimmed_line;
            }
        }

        if (line == NULL || *line == EOF)
        {
            g_exit = 0;
            break;
        }

        if (ft_str_empty(line))
        {
            free(line);
            continue;
        }

        if (line[0] != '\0')
            add_history(line);

        if (syntax_error_check(macro, line))
        {
            macro->exit_code = 2;
            free(line);
            continue;
        }

        if (g_exit > 0)
        {
            macro->exit_code = g_exit;
            g_exit = 0;
        }

        macro->ins = line;
        tokenizer(macro);
        parsing(macro);
        if (macro->cmds == NULL)
        {
            free(line);
            continue;
        }

        execution(macro);
        free_ins(macro);
    }

    free_macro(macro);
    exit(0);
}

