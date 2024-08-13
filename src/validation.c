/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 22:35:57 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/13 23:12:04 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file(t_token *token)
{
	int	fd;

	// if (macro->here_doc == 1)
	//  macro->out_fd = open(macro->outfile, O_WRONLY | O_CREAT | O_APPEND,0644);
	// else
	if (token->type == INRED)
		fd = open(token->value, O_RDONLY);
	else if (token->type == OUTRED)
		fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (token->type == APPEND)
		fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (errno == ENOENT)
		return (-1);
	if (errno == EACCES)
		return (-1);
	if (errno == IS_DIRECTORY)
		return (-1);
	return (fd);
}

int	open_last_redir_file(t_token *redir)
{
	t_token	*tmp;
	int		fd;
	t_type	type;

	tmp = redir;
	fd = -1;
	while (tmp)
	{
		if (tmp->type == HERE_DOC)
		{
			tmp = tmp->next;
			continue ;
		}
		fd = open_file(tmp);
		if (fd == -1)
			break ;
		if (tmp->next != NULL)
		{
			close(fd);
			fd = -1;
		}
		tmp = tmp->next;
	}
	return (fd);
}

void	dup_stdout(t_macro *macro, t_cmd *cmd)
{
	int	fd;

	fd = open_last_redir_file(cmd->out_redir);
	if (fd >= 1)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			close(fd);
			return ;
		}
		close(fd);
	}
	else if (cmd->n < macro->num_cmds)
	{
		if (dup2(macro->pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 pipe write");
			return ;
		}
	}
	close(macro->pipe_fd[1]);
}

void	dup_stdin(t_macro *macro, t_cmd *cmd, int read_end)
{
	int	fd;

	// add here_doc fd here if the only one
	fd = open_last_redir_file(cmd->in_redir);
	if (fd >= 1)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			close(fd);
			return ;
		}
		close(fd);
	}
	else if (cmd->n > 1)
	{
		if (dup2(read_end, STDIN_FILENO) < 0)
		{
			perror("dup2 pipe read");
			return ;
		}
	}
	close(macro->pipe_fd[0]);
}

void	dup_file_descriptors(t_macro *macro, t_cmd *cmd, int read_end)
{
	dup_stdout(macro, cmd);
	dup_stdin(macro, cmd, read_end);
}

bool	is_directory(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		return (true);
	}
	return (false);
}

int	validate_access(char *file)
{
	if (!access(file, F_OK))
	{
		if (!access(file, X_OK))
		{
			if (is_directory(file))
				return(error_msg(file, 126));
			return(0);
		}
		return(error_msg(file, 126));		
	}
	return(error_msg(file, 127));
}

static char	**parse_paths(char **env)
{
	int		i;
	char	**paths;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			paths = ft_split(env[i] + 5, ':');
			return (paths);
		}
		i++;
	}
	return (NULL);
}

char	*get_executable_path(t_macro *macro, char **paths, char *executable)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], executable, "/");
		if (full_path == NULL)
			return (NULL);
		if (!access(full_path, F_OK))
			return (full_path);
		free(full_path);
		i++;
	}
	ft_putstr_fd("Executable not found\n", 2);
	return (NULL);
}



int	validate_executable(t_macro *macro, t_cmd *cmd)
{
	char	*executable;
	char	**paths;
	char	*full_path;

	if (ft_strchr("./", cmd->cmd_arg->value[0]) == NULL)
	{
		executable = ft_strdup(cmd->cmd_arg->value);
		if (!executable)
			return (-1);
		paths = parse_paths(macro->env);
		if (!paths)
		{
			free(executable);
			return (-1);
		}
		full_path = get_executable_path(macro, paths, executable);
		if (full_path)
		{
			free(cmd->cmd_arg->value);
			cmd->cmd_arg->value = full_path;
		}
		free(paths);
		free(executable);
	}
	return(validate_access(cmd->cmd_arg->value));
}


