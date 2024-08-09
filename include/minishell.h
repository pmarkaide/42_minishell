/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:11:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/09 20:31:31 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h" /* libft library */
# include <limits.h>             /* for LONG_MAX, LONG_MIN */
# include <stdbool.h>            /* for true and false*/
# include <stdlib.h>             /* for malloc, free, exit, getenv */
# include <errno.h>              /* for errno */
# include <fcntl.h>              /* for open, O_DIRECTORY */
# include <sys/wait.h>           /* for waitpid, wait, */
# include <unistd.h>             /* for file r/w, dup2, execve, fork, pipe */

# define NO_FILE 1
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
# define SEGFAULT 139
# define IS_DIRECTORY -1
# define EXEC_NOT_FOUND -2

typedef enum e_type
{
	STRING,
	CMD,
	PIPE,
	INRED,
	OUTRED,
	HERE_DOC,
	APPEND,
	BUILTIN,
	ARG
}					t_type;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd
{
	int			n;
	t_type			type;
	t_token			*cmd_arg;
	t_token			*redir;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_macro
{
	char			**envp;
	char			**env;
	char			**path;
	char			**history;
	char			*instruction;
	t_token			*tokens;
	t_cmd			*cmds;
	int				num_cmds;
	int				pipe_fd[2];
	int				in_fd;
	int				out_fd;
	pid_t			*pid;
}					t_macro;

/* presyntax*/
char				*syntax_error_check(char *instruction);

/* tokenizer */
void				tokenizer(t_macro *macro);

/* tokenizer_utils */
bool				is_inside_single_quotes(const char *str, int index);
char				*expand_envirs(char *clean, char *instruction);
bool				is_builtin(t_token *token);
bool				is_redir(t_token *token);

/* list_utils */
t_token				*init_token(void);
void				token_add_back(t_token **tokens, t_token *new);
t_token				*last_token(t_token *token);
void				free_tokens(t_token **tokens);
void				print_tokens(t_token *tokens);
t_cmd				*init_cmd(void);
void				cmd_add_back(t_cmd **cmds, t_cmd *new);
t_cmd				*last_cmd(t_cmd *cmd);
void				print_cmds(t_cmd *cmds);
char				*enum_to_char(t_type type);
bool				is_last_of_type(t_token *tokens, t_type type);
int					tokens_size(t_token *tokens);


/* parsing */
t_cmd				*parsing(t_token *tokens);

/* execution */
int					execution(t_macro *macro);

/* execution utils */
char				**build_cmd_args_array(t_token *cmd_args);

/* validation */
int			open_infile(char *infile);
int			open_infile(char *outfile);
void		close_open_fds(t_macro *macro);
void		dup2_or_exit(t_macro *macro, int oldfd, int newfd);
void		dup_file_descriptors(t_macro *macro, t_cmd *cmd, int read_end);


/* tests */
char				*get_envir_value(const char *str, int *len);
size_t				expanded_envir_len(char *instruction);

/* free */
void	free_array(char ***array);
void	free_tokens(t_token **tokens);

#endif /* MINISHELL_H */
