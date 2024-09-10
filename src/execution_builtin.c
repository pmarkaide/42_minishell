/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 09:15:15 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/10 14:33:46 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	select_and_run_builtin(char *cmd, char **args, t_macro *macro)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ft_echo2(args));
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (ft_cd(args, macro));
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (ft_pwd2(macro));
	if (ft_strncmp(cmd, "export", 6) == 0)
		return (ft_export2(args, macro));
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (ft_unset2(args, macro));
	if (ft_strncmp(cmd, "env", 3) == 0)
		return (ft_env2(macro));
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (ft_exit2(args, macro));
	return (0);
}

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
	close_fds(macro);
	return (macro->exit_code);
}

void	execute_builtin(t_macro *macro, char **cmd_array)
{
	macro->exit_code = select_and_run_builtin(cmd_array[0], cmd_array, macro);
	free_array(&cmd_array);
}
