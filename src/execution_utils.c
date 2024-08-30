/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:03:14 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 23:59:14 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_macro *macro, int read_end)
{
	if (macro->pipe_fd[0] != -1)
	{
		close(macro->pipe_fd[0]);
		macro->pipe_fd[0] = -1;
	}
	if (macro->pipe_fd[1] != -1)
	{
		close(macro->pipe_fd[1]);
		macro->pipe_fd[1] = -1;
	}
	if (macro->pipe_exit[0] != -1)
	{
		close(macro->pipe_exit[0]);
		macro->pipe_exit[0] = -1;
	}
	if (macro->pipe_exit[1] != -1)
	{
		close(macro->pipe_exit[1]);
		macro->pipe_exit[1] = -1;
	}
	if (read_end > 0)
		close(read_end);
}

void	read_pipe_exit(int *pipe_exit, int *status)
{
	if (pipe_exit == NULL || status == NULL)
    {
        perror("read_pipe_exit :: invalid arguments");
        return;
    }
    close(pipe_exit[1]);
    read(pipe_exit[0], status, sizeof(int));
    // if (read(pipe_exit[0], status, sizeof(int)) == -1)
    //     perror("read_pipe_exit :: read error");
    close(pipe_exit[0]);
}

void	write_pipe_exit(int *pipe_exit, int status)
{
	if (pipe_exit == NULL)
    {
        perror("write_pipe_exit :: invalid arguments");
        return;
    }
    close(pipe_exit[0]);
    if (write(pipe_exit[1], &status, sizeof(int)) == -1)
        perror("write_pipe_exit :: write error");
    close(pipe_exit[1]);
}

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

char	**build_cmd_args_array(t_token *cmd_args)
{
	char	**cmd_array;
	int		i;
		
    if (!cmd_args)
        return (NULL);
    cmd_array = (char **)malloc(sizeof(char *) * (tokens_size(cmd_args) + 1));
    if (!cmd_array)
        return(NULL);
    i = 0;
    while (cmd_args)
    {
        cmd_array[i] = ft_strdup(cmd_args->value);
        if (!cmd_array[i])
        {
            free_array(&cmd_array);
            return(NULL);
        }
        i++;
        cmd_args = cmd_args->next;
    }
    cmd_array[i] = NULL;
    return (cmd_array);
}
