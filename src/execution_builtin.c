/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 09:15:15 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/04 13:41:56 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	restore_fds(int saved_stdout, int saved_stdin)
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

static int	prepare_exec(t_macro *macro, char ***cmd_array, int *out, int *in)
{
	*out = dup(STDOUT_FILENO);
	*in = dup(STDIN_FILENO);
	if (*in == -1 || *out == -1)
	{
		error_msg(macro, "dup", -1);
		return (-1);
	}
	if (validate_redirections(macro->cmds->redir, macro) == -1)
	{
		restore_fds(*out, *in);
		return (1);
	}
	if (dup_file_descriptors(macro, macro->cmds) == -1)
	{
		restore_fds(*out, *in);
		return (-1);
	}
	*cmd_array = build_cmd_args_array(macro->cmds->cmd_arg);
	if (*cmd_array == NULL)
	{
		error_msg(macro, "malloc", -1);
		restore_fds(*out, *in);
		return (-1);
	}
	return (0);
}

int	execute_single_builtin(t_macro *macro)
{
	char	**cmd_array;
	int		saved_stdout;
	int		saved_stdin;
	int		prep_status;

	prep_status = prepare_exec(macro, &cmd_array, &saved_stdout, &saved_stdin);
	if (prep_status != 0)
		return (prep_status);
	execute_builtin(macro, cmd_array);
	restore_fds(saved_stdout, saved_stdin);
	return (macro->exit_code);
}

void	execute_builtin(t_macro *macro, char **cmd_array)
{
	macro->exit_code = select_and_run_builtin(cmd_array[0], cmd_array, macro);
	free_array(&cmd_array);
}
