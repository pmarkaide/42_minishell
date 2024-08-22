/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:23:53 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/22 23:36:51 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit;

int	execute_builtin(t_macro *macro, char **cmd_array)
{
	char	*builtin;

	builtin = remove_path(cmd_array[0]);
	g_exit = select_and_run_builtin(builtin, cmd_array, macro);
	// if (g_exit != 0)
	// 	ft_putstr_fd("builtin failed\n", 2);
	return (g_exit);
}

static char	**prepare_child_execution(t_macro *macro, t_cmd *cmd)
{
	char	**cmd_array;

	if (cmd->type == CMD)
		validate_executable(macro, cmd);
	cmd_array = build_cmd_args_array(cmd->cmd_arg);
	if (!cmd_array)
		exit(errno);
	return (cmd_array);
}

static void	execute_child_process(t_macro *macro, int index, int read_end, int pipe_exit[2], int total_cmds)	
{
	int		i;
	t_cmd	*cmd;
	char	**cmd_array;

	close(pipe_exit[0]);
	cmd = macro->cmds;
	i = 0;
	while (cmd != NULL && i++ < index)
		cmd = cmd->next;
	dup_file_descriptors(macro, cmd, read_end);
	cmd_array = prepare_child_execution(macro, cmd);
	if (cmd->type == BUILTIN)
		g_exit = execute_builtin(macro, cmd_array);
	else
		execve(cmd_array[0], cmd_array, macro->env);
	//g_exit = errno;
	//printf("g_exit loca0: %d\neres el pid %d\n", g_exit, getpid());
	// if (g_exit != 0)
	// 	ft_putstr_fd("execution failed\n", 2);
	if (index == total_cmds - 1)
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
			close(macro->pipe_fd[1]);
			return (error_msg("fork failed", i));
		}
		else if (macro->pid[i] == 0)
			execute_child_process(macro, i, read_end, pipe_exit, macro->num_cmds);
		if (read_end > 0)
			close(read_end);
		read_end = macro->pipe_fd[0];
		close(macro->pipe_fd[1]);
		i++;
	}
			if (macro->pid != 0)
		{
			close (pipe_exit[1]);
			read(pipe_exit[0], &g_exit, sizeof(int));	
			//printf("g_exit loca1: %deres el pid: %d\n", g_exit, getpid());
			close(pipe_exit[0]);
		} 
	//printf("g_exit loca2: %deres el pid: %d\n", g_exit, getpid());
	if (read_end > 0)
		close(read_end);
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
	{
		cmd_array = build_cmd_args_array(macro->cmds->cmd_arg);
		if (cmd_array == NULL)
			return (1);
		g_exit = execute_builtin(macro, cmd_array); // wait and pid ???
		free_array(&cmd_array);
	}
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
		{
			close (pipe_exit[1]);
			read(pipe_exit[0], &g_exit, sizeof(int));
			close(pipe_exit[0]);
		} 
		free(macro->pid);
	}
	return (g_exit);
}
