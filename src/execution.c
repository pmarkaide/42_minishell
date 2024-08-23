/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/23 12:33:42 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

int	execute_builtin(t_macro *macro, char **cmd_array)
{
	char	*builtin;

	builtin = remove_path(cmd_array[0]);
	g_exit = select_and_run_builtin(builtin, cmd_array, macro);
	return (g_exit);
}

int execute_single_builtin(t_macro *macro)
{
	char **cmd_array;
	int saved_stdout;
	int saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	cmd_array = build_cmd_args_array(macro->cmds->cmd_arg);
	if (cmd_array == NULL)
		return (1);
	if(validate_redirections(macro->cmds->redir) == -1)
		return(-1);
	dup_file_descriptors(macro, macro->cmds, 0);
	g_exit = execute_builtin(macro, cmd_array);
	free_array(&cmd_array);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	return g_exit;
}

static void	execute_child_process(t_macro *macro, int index, int read_end, int pipe_exit[2])
{
	int		i;
	t_cmd	*cmd;
	char	**cmd_array;

	close(pipe_exit[0]);
	cmd = macro->cmds;
	i = 0;
	while (cmd != NULL && i++ < index)
		cmd = cmd->next;
	if(validate_redirections(cmd->redir) == -1)
		exit(g_exit);
	dup_file_descriptors(macro, cmd, read_end);
	cmd_array = prepare_child_execution(macro, cmd);
	if (cmd->type == BUILTIN)
		g_exit = execute_builtin(macro, cmd_array);
	else
		execve(cmd_array[0], cmd_array, macro->env);
	if (index == macro->num_cmds - 1)
	{
		write(pipe_exit[1], &g_exit, sizeof(int));
		close(pipe_exit[1]);
	}
	exit(g_exit);
}


static int	execute_cmds(t_macro *macro, int read_end, int pipe_exit[2])
{
	int	i;

	i = 0;
	while (i < macro->num_cmds)
	{
		if (pipe(macro->pipe_fd) == -1)
			return (error_msg("pipe failed", i));
		macro->pid[i] = fork();
		if (macro->pid[i] < 0)
		{
			close_fds(macro->pipe_fd, read_end);
			return (error_msg("fork failed", i));
		}
		else if (macro->pid[i] == 0)
			execute_child_process(macro, i, read_end, pipe_exit);
		if (read_end > 0)
			close(read_end);
		read_end = macro->pipe_fd[0];
		close(macro->pipe_fd[1]);
		i++;
	}
	if (macro->pid != 0)
		catch_parent_exit(pipe_exit, &g_exit);
	return (i);
}

int	execution(t_macro *macro)
{
	int		read_end;
	int		num_cmds_executed;
	char	**cmd_array;
	int	 	pipe_exit[2];
	int 	i;

	if (macro->num_cmds == 1 && macro->cmds->type == BUILTIN)
		execute_single_builtin(macro);
	else
	{
		if (pipe(pipe_exit) == -1)
			return (error_msg("pipe failed", 0));
		read_end = 0;
		macro->pid = malloc(sizeof(pid_t) * macro->num_cmds);
		num_cmds_executed = execute_cmds(macro, read_end, pipe_exit);
		i = 0;
		while (i < num_cmds_executed)
			g_exit = wait_processes(macro->pid[i++]);
		if (macro->pid != 0)
			catch_parent_exit(pipe_exit, &g_exit);
		free(macro->pid);
	}
	return (g_exit);
}
