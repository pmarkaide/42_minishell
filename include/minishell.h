/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:11:45 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/02 15:09:21 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h" /* libft library */
# include <fcntl.h>              /* for open */
# include <limits.h>             /* for LONG_MAX, LONG_MIN */
# include <stdlib.h>             /* for malloc, free, exit */
# include <unistd.h>             /* for read, write */

typedef enum e_type
{
	STRING,
	CMD,
	PIPE,
	INRED,
	OUTRED,
	HERE_DOC,
	APPEND,
	SEMICOLON,
	NEWLINE
}			t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
	t_token	*next;
}			t_token;

typedef struct s_macro
{
	char	**envp;
	char	**env;
	char	**path;
	char	**history;
	char	*instruction;
	t_token	**tokens;
	char	***commands;
}			t_macro;

#endif /* MINISHELL_H */