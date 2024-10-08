/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:11:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/15 20:55:35 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h" /* libft library */
# include <dirent.h>             // opendir, readdir, closedir
# include <errno.h>              /* for errno */
# include <readline/history.h>   // add_history
# include <readline/readline.h>  // readline, rl_clear_history, rl_on_new_line
# include <signal.h>             // signal, sigaction, sigemptyset, sigaddset
# include <stdbool.h>            /* for true and false*/
# include <stdlib.h>             // malloc, free, exit, getenv
# include <sys/types.h>          // fork, wait, waitpid, wait3, wait4,stat
# include <sys/wait.h>           // wait, waitpid, wait3, wait4
# include <unistd.h>             // read, write, access, open, close,fork

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
	char			**env;
	char			*ins;
	t_token			*tokens;
	t_cmd			*cmds;
	int				num_cmds;
	int				pipe_fd[2];
	int				read_end;
	pid_t			*pid;
	char			*m_pwd;
	char			*m_home;
	int				exit_code;
	int				exit_flag;
	int				here_doc_flag;
}					t_macro;

/* buin_cd_utils */
char				*get_home_directory(t_macro *macro);
char				*parse_arguments(char **args, t_macro *macro, char *home);
int					change_directory(char *path, char *home);
void				update_environment(t_macro *macro, char *oldpwd,
						char *path);

/* buin_cd */
int					ft_cd(char **args, t_macro *macro);

/* buin_echo */
int					ft_echo(char **args);

/* buin_env */
char				*grab_env(char *var, char **env, int n);
char				**fix_env(char *var, char *value, char **env, int n);
int					ft_env(t_macro *macro);

/* buin_exit */
int					ft_exit(char **args, t_macro *macro);

/* buin_export_utils */
int					validate_and_clean_argument(char *arg, int *exit_flag);
void				export_argless(t_macro *macro);

/* buin_export */
int					ft_export(char **args, t_macro *macro);

/* buin_pwd */
char				*char_pwd(void);
int					ft_pwd(t_macro *macro);

/* buin_unset */
int					ft_unset(char **args, t_macro *macro);

/* dup */
int					dup_file_descriptors(t_macro *macro, t_cmd *cmd);

/* env */
int					ft_strchr_i(const char *s, int c);
char				*ft_getenv(char *var, char **env);

/* error */
int					error_msg(t_macro *macro, char *file, int exit_code);
void				exit_error(char *file, char *msg, t_macro *macro,
						int exit_code);
void				exit_free(t_macro *macro);

/* execution_builtin */
int					execute_single_builtin(t_macro *macro);
void				execute_builtin(t_macro *macro, char **cmd_array);

/* execution_utils */
void				close_fd(int *fd);
void				close_fds(t_macro *macro);
int					wait_processes(pid_t pid);
char				**build_cmd_args_array(t_token *cmd_args);

/* execution */
void				execution(t_macro *macro);

/* expand_cases */
void				handle_normal_char(char **clean, char *ins, size_t *i);
void				handle_quoted_literal(char **clean, char *ins, size_t *i);
void				handle_unexpected_case(char **clean, char *ins, size_t *i);
void				handle_delimiter_after_dollar(char **clean, char *ins,
						size_t *i);

/* expand_utils */
bool				envir_must_be_expanded(char *str, int index);
bool				is_in_quote(char *str, int index);
bool				inside_double_quotes(const char *str, int index);

/* expand */
char				*get_expanded_ins(char *ins, t_macro *macro);
char				*get_expanded_doc(char *ins, t_macro *macro);

/* free_utils */
char				*free_2_strings(char **str1, char **str2);
char				*free_array(char ***array);
char				*free_string(char **str);

/* free */
t_token				*free_tokens(t_token **tokens);
t_cmd				*free_cmds(t_cmd **cmds);
t_macro				*free_ins(t_macro *macro);
t_macro				*free_macro(t_macro *macro);
void				ft_free_matrix(char ***matrix);

/* general_utils */
bool				type_is_redirection(t_type type);
char				*ft_strjoin3(const char *s1, const char *s2,
						const char *s3);
t_macro				*init_macro(char **envp, char **argv);

/* init_cmd */
t_cmd				*init_cmd(void);
void				cmd_add_back(t_cmd **cmds, t_cmd *new);
t_cmd				*last_cmd(t_cmd *cmd);

/* init_token */
t_token				*init_token(void);
void				token_add_back(t_token **tokens, t_token *new);
t_token				*remove_token(t_token **tokens, t_token *token);
t_token				*last_token(t_token *token);
int					tokens_size(t_token *tokens);

/* main_path */
char				*create_path(t_macro *macro);

/* main_utils */
char				**ft_add_row(char **input, char *to_add);
char				**copy_env(char **envp);

/* parsing_here_doc */
int					process_cmd(t_cmd *cmd, t_macro *macro);

/* parsing_utils  */
char				parsing_error_check(t_token *tokens, t_macro *macro);
t_cmd				*parse_tokens(t_token *tokens, int *n);

/* parsing */
int					parsing(t_macro *macro);

/* presyntax */
int					syntax_error_check(t_macro *macro, char *ins);

/* signals */
void				sigint_handler_in_child(int sig);
void				sigint_handler_in_parent(int sig);
void				sigint_handler_after_here_doc(int sig);
void				sigint_handler_here_doc(int sig);
void				sigquit_handler_in_child(int sig);

/* split_args */
t_list				*split_args_by_quotes(char *ins);

/* tokenizer_expand */
t_token				*expand_arg_tokens(t_macro *macro);
t_token				*remove_empty_envir_tokens(t_macro *macro);
void				ensure_at_least_one_cmd(t_token **tokens);

/* tokenizer_retokenize */
t_token				*handle_retokenize(char *expanded, t_token *token,
						t_macro *macro);

/* tokenizer_utils */
void				fix_redirections(char *ins);
bool				is_redir(t_token *token, char *redir_type);
bool				is_builtin(t_token *token);
char				*clean_quotes(char *str);
int					clean_token_quotes(t_token *tokens);

/* tokenizer */
int					tokenizer(t_macro *macro);

/* validation_utils */
int					open_file(t_token *token, t_macro *macro);
bool				is_directory(const char *path);
char				**parse_paths(char **env);
char				*get_executable_path(char **paths, char *executable,
						t_macro *macro);

/* validation */
int					validate_redirections(t_token *redir, t_macro *macro);
void				validate_access(char *exec, t_macro *macro);
void				validation(t_macro *macro, t_cmd *cmd);

/* zz_print_utils*/
void				print_tokens(t_token *tokens);
void				print_cmds(t_cmd *cmds);

#endif /* MINISHELL_H */
