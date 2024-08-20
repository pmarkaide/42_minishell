/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/21 02:03:59 by dbejar-s         ###   ########.fr       */
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
