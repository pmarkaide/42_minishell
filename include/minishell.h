/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:11:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/09 22:14:38 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h" /* libft library */
# include <limits.h>             /* for LONG_MAX, LONG_MIN */
# include <errno.h>              /* for errno */
# include <stdbool.h>			 /* for true and false*/
# include <stdio.h>      // printf, perror
# include <stdlib.h>     // malloc, free, exit, getenv
# include <unistd.h>     // read, write, access, open, close, fork, getcwd, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <fcntl.h>      // open
# include <sys/types.h>  // fork, wait, waitpid, wait3, wait4, stat, lstat, fstat
# include <sys/wait.h>   // wait, waitpid, wait3, wait4
# include <sys/stat.h>   // stat, lstat, fstat
# include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill
# include <dirent.h>     // opendir, readdir, closedir
# include <string.h>     // strerror
# include <termios.h>    // tcsetattr, tcgetattr
# include <curses.h>     // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <term.h>       // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/ioctl.h>  // ioctl
# include <readline/readline.h>  // readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>  // add_history

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

typedef enum e_builtin
{
	M_ECHO = 0,
	M_CD = 1,
	M_PWD = 2,
	M_EXPORT = 3,
	M_UNSET = 4,
	M_ENV	= 5,
	M_EXIT = 6
}	t_builtin;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd
{
	int				n;
	t_type			type;
	t_token			*cmd_arg;
	t_token			*redir;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_macro
{
	char			**envp;
	char			**env;
	char			*path;
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
int				syntax_error_check(char *instruction);

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
t_cmd				*parsing(t_macro *macro);

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

/* others */
void				ft_signal_handler(int signum);
void			    test_builtins(t_macro *macro);
void    			ft_pwd(void);
void    			ft_echo(char *line);
void    			ft_echo_n(char *line);
void			    ft_env(char **envp);
void				ft_export(t_macro *macro);
void				ft_exit(t_macro *macro);
void				ft_cd(char *line);
char    			*char_pwd(void);
char    			**copy_env(char **envp);
void				ft_free_matrix(char ***m);
int					ft_matrixlen(char **m);
char				**ft_add_row(char **in, char *newstr);
void				ft_export_do(t_macro *macro, char *name, char *value);
int					ft_strchr_i(const char *s, int c);
char				**ft_replace_matrix_row(char ***big, char **small, int n);
void				ft_unset(t_macro *macro);
int					var_in_env(char *argv, char **env, int ij[2]);

#endif /* MINISHELL_H */
