/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 21:45:16 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// extern int	g_exit;

// int	get_exit_code(int status)
// {
// 	int	exit_code;

// 	exit_code = 0;
// 	if (WIFSIGNALED(status))
// 		exit_code = 128 + WTERMSIG(status);
// 	else if (WIFEXITED(status))
// 		exit_code = WEXITSTATUS(status);
// 	return (exit_code);
// }

// int	wait_processes(pid_t *pid, int cmds)
// {
// 	int	i;
// 	int	status;
// 	int	exit_code;

// 	i = 0;
// 	exit_code = 0;
// 	while (i < cmds)
// 	{
// 		waitpid(pid[i], &status, 0);
// 		if (i == cmds - 1)
// 			exit_code = get_exit_code(status);
// 		i++;
// 	}
// 	return (exit_code);
// }

#include "minishell.h"


char	*clean_quotes(char *str)
{
    char	*result;
    int		i;
    int		j;
    bool	double_quote_open;
    bool	single_quote_open;

    result = (char *)malloc(ft_strlen(str) + 1);
    if (!result)
        return (NULL);
    double_quote_open = false;
    single_quote_open = false;
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '\"' && !single_quote_open)
            double_quote_open = !double_quote_open;
        else if (str[i] == '\'' && !double_quote_open)
            single_quote_open = !single_quote_open;
        else
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    return (result);
}

char	**build_cmd_args_array(t_token *cmd_args)
{
	char	**cmd_array;
	t_token	*tmp;
	int		i;

	tmp = cmd_args;
	cmd_array = (char **)malloc(sizeof(char *) * (tokens_size(cmd_args) + 1));
	if (!cmd_array)
		return (NULL);
	i = 0;
	tmp = cmd_args;
	while (tmp)
	{
		cmd_array[i] = ft_strdup(tmp->value);
		cmd_array[i] = clean_quotes(cmd_array[i]);
		if (!cmd_array[i])
		{
			free_array(&cmd_array);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}


char	**prepare_child_execution(t_macro *macro, t_cmd *cmd)
{
	char	**cmd_array;

	if (cmd->type == CMD)
		validate_executable(macro, cmd);
	cmd_array = build_cmd_args_array(cmd->cmd_arg);
	if (!cmd_array)
		exit(errno);
	return (cmd_array);
}

