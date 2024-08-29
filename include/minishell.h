/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:11:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/29 22:12:13 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h" /* libft library */
# include <curses.h>             // tgetent, tgetflag, tgetnum, tgetstr,
# include <dirent.h>             // opendir, readdir, closedir
# include <errno.h>              /* for errno */
# include <fcntl.h>              // open
# include <limits.h>             /* for LONG_MAX, LONG_MIN */
# include <readline/history.h>   // add_history
# include <readline/readline.h>  // readline, rl_clear_history,rl_on_new_line
# include <signal.h>             // signal, sigaction, sigemptyset,sigaddset
# include <stdbool.h>            /* for true and false*/
# include <stdio.h>              // printf, perror
# include <stdlib.h>             // malloc, free, exit, getenv
# include <string.h>             // strerror
# include <sys/ioctl.h>          // ioctl
# include <sys/stat.h>           // stat, lstat, fstat
# include <sys/types.h>          // fork, wait, waitpid, wait3, wait4,stat
# include <sys/wait.h>           // wait, waitpid, wait3, wait4
# include <term.h>               // tgetent, tgetflag, tgetnum,tgetstr,tgoto
# include <termios.h>            // tcsetattr, tcgetattr
# include <unistd.h>             // read, write, access, open,close,fork

# define NO_FILE 1
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
# define SEGFAULT 139
# define IS_DIRECTORY 21
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
	M_ENV = 5,
	M_EXIT = 6
}					t_builtin;

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
	char			**history;
	char			*instruction;
	t_token			*tokens;
	t_cmd			*cmds;
	int				num_cmds;
	int				pipe_fd[2];
	pid_t			*pid;
	char			*m_pwd;
	char			*m_home;
	int				exit_code;
	int				pipe_exit[2];
}					t_macro;

/* presyntax*/
int					syntax_error_check(char *instruction);

/* tokenizer */
void				tokenizer(t_macro *macro);
t_list				*split_args_by_quotes(char *ins);

/* tokenizer_expand */
t_token				*expand_arg_tokens(t_macro *macro);
void				ensure_at_least_one_cmd(t_token **tokens);

/* tokenizer_utils */
char				*expand_envir(char *clean, char *instruction,
						t_macro *macro);
bool				is_builtin(t_token *token);
bool				is_redir(t_token *token, char *redir_type);
void				fix_redirections(char *instruction);
void				clean_token_quotes(t_token *tokens);
char				*clean_quotes(char *str);

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
void				remove_token(t_token **tokens, t_token *token);
t_token				*remove_empty_envir_tokens(t_macro *macro);

/* parsing */
t_cmd				*parsing(t_macro *macro);

/* parsing utils */
void				handle_here_doc(t_cmd *cmds, t_macro *macro);
void				close_here_doc_not_needed(t_token *tokens);

/* execution */
void				execution(t_macro *macro);

/* execution utils */
char				**build_cmd_args_array(t_token *cmd_args, t_macro *macro);
char				**prepare_child_execution(t_macro *macro, t_cmd *cmd);
int					wait_processes(pid_t pid);
void				catch_parent_exit(int *pipe_exit, int *exit_code);
void				close_fds(t_macro *macro, int read_end);

/* validation */
void				validation(t_macro *macro, t_cmd *cmd);
int					validate_redirections(t_token *redir, t_macro *macro);
void				validate_access(char *exec, t_macro *macro);
void				search_executable(t_macro *macro, t_cmd *cmd);

/* validation utils */
bool				is_directory(const char *path);
char				**parse_paths(char **env);
char				*get_executable_path(char **paths, char *executable,
						t_macro *macro);
int					open_file(t_token *token, t_macro *macro);

/* expand */
char				*get_expanded_instruction(char *instruction,
						t_macro *macro);

/* dup */
void				dup_file_descriptors(t_macro *macro, t_cmd *cmd,
						int read_end);

/* clean utils*/
char				*get_envir_name(char *str);
char				*get_envir_value(char *str, t_macro *macro);
bool				envir_must_be_expanded(char *instruction, int index);
bool				is_in_quote(char *str, int index);

/* free */
void				free_string(char **str);
void				free_array(char ***array);
void				free_tokens(t_token **tokens);
void				free_ins(t_macro *macro);
void				free_macro(t_macro *macro);
void				free_cmds(t_cmd **cmds);

/* others */
void				ft_signal_handler(int signum);
char				*char_pwd(void);
char				**copy_env(char **envp);
void				ft_free_matrix(char ***m);
int					ft_matrixlen(char **m);
char				**ft_add_row(char **in, char *newstr);
void				ft_export_do(t_macro *macro, char *name, char *value);
int					ft_strchr_i(const char *s, int c);
char				**ft_replace_matrix_row(char ***big, char **small, int n);
void				ft_unset(t_macro *macro);
int					var_in_env(char *argv, char **env, int ij[2]);
int					select_and_run_builtin(char *cmd, char **args,
						t_macro *macro);
char				*grab_env(char *var, char **env, int n);
char				**fix_env(char *var, char *value, char **env, int n);
char				*remove_path(char *cmd);
t_macro				*init_macro(char **envp, char **argv);
t_macro				*start_env(t_macro *macro, char **argv);
char				*ft_getenv(char *var, char **env);
int					ft_pwd2(t_macro *macro);
int					ft_env2(t_macro *macro);
int					ft_exit2(char **args);
int					ft_unset2(char **args, t_macro *macro);
int					ft_export2(char **args, t_macro *macro);
int					ft_echo2(char **args);
int					ft_cd2(char **args, t_macro *macro);
char				*ft_strjoin3(const char *s1, const char *s2,
						const char *s3);
void				free_2_strings(char **str1, char **str2);
char				*get_home_directory(t_macro *macro);
char				*parse_arguments(char **args, t_macro *macro, char *home);
int					change_directory(char *path, char *home);
void				check_save_env(char *var, t_macro *macro, int size);
void				update_environment(t_macro *macro, char *oldpwd,
						char *path);
void				print_export_var(char *var, char *value);
void				export_argless(t_macro *macro);
int					check_export(char *arg);
char				*remove_quotes(char *str);
t_macro				*init_macro(char **envp, char **argv);
char				*grab_home(t_macro *macro);
t_macro				*start_env(t_macro *macro, char **argv);
int					in_root(char *path);
int					in_home(t_macro *macro);
char				*upper_than_home(t_macro *macro);
char				*create_path(t_macro *macro);

/* error */
int					error_msg(char *msg, int exit_code);
void				exit_error(char *file, char *msg, t_macro *macro,
						int exit_code);

#endif /* MINISHELL_H */
