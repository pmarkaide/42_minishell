/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 23:33:26 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_processes(pid_t pid)
{
	int	status;
	int	exit_code;

	exit_code = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status); 
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	return (exit_code);
}


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

