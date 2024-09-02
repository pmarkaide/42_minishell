/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 09:15:15 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/02 09:15:21 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(t_macro *macro, char **cmd_array)
{
	char	*builtin;

	builtin = remove_path(cmd_array[0]);
	macro->exit_code = select_and_run_builtin(builtin, cmd_array, macro);
	free_array(&cmd_array);
}

int	restore_fds(int saved_stdout, int saved_stdin)
{
	int	result;

	result = 0;
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2 :: restore_fds");
		result = -1;
	}
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2 :: restore_fds");
		result = -1;
	}
	close(saved_stdout);
	close(saved_stdin);
	return (result);
}

int	execute_single_builtin(t_macro *macro)
{
	char	**cmd_array;
	int		saved_stdout;
	int		saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		error_msg(macro, "dup", -1);
		return (-1);
	}
	if (validate_redirections(macro->cmds->redir, macro) == -1)
		return (1);
	if (dup_file_descriptors(macro, macro->cmds, 0) == -1)
		return (-1);
	cmd_array = build_cmd_args_array(macro->cmds->cmd_arg);
	if (cmd_array == NULL)
	{
		error_msg(macro, "malloc", -1);
		restore_fds(saved_stdout, saved_stdin);
	}
	else
		execute_builtin(macro, cmd_array);
	restore_fds(saved_stdout, saved_stdin);
	return (0);
}
