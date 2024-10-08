/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:42:44 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/09/14 20:13:50 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*parse_redir_tokens(t_token *tokens)
{
	t_token	*tmp;
	t_token	*token;
	t_token	*redir;

	tmp = tokens;
	redir = NULL;
	while (tmp && tmp->type != PIPE)
	{
		if (is_redir(tmp, "input") || is_redir(tmp, "output"))
		{
			token = init_token();
			if (!token)
				return (NULL);
			token->type = tmp->type;
			token->value = ft_strdup(tmp->value);
			if (!token->value)
			{
				free(token);
				return (NULL);
			}
			token_add_back(&redir, token);
		}
		tmp = tmp->next;
	}
	return (redir);
}

static t_token	*parse_cmd_arg_tokens(t_token *tokens)
{
	t_token	*tmp;
	t_token	*token;
	t_token	*cmd_arg;

	tmp = tokens;
	cmd_arg = NULL;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == CMD || tmp->type == BUILTIN || tmp->type == ARG)
		{
			token = init_token();
			if (!token)
				return (NULL);
			token->type = tmp->type;
			token->value = ft_strdup(tmp->value);
			if (!token->value)
			{
				free(token);
				return (NULL);
			}
			token_add_back(&cmd_arg, token);
		}
		tmp = tmp->next;
	}
	return (cmd_arg);
}

t_cmd	*parse_tokens(t_token *tokens, int *n)
{
	t_cmd	*cmds;
	t_cmd	*cmd;
	t_token	*tmp;

	cmds = NULL;
	tmp = tokens;
	while (tmp)
	{
		cmd = init_cmd();
		if (!cmd)
			return (NULL);
		cmd->n = (*n)++;
		cmd->cmd_arg = parse_cmd_arg_tokens(tmp);
		if (cmd->cmd_arg)
			cmd->type = cmd->cmd_arg->type;
		cmd->redir = parse_redir_tokens(tmp);
		cmd_add_back(&cmds, cmd);
		while (tmp && tmp->type != PIPE)
			tmp = tmp->next;
		if (tmp)
			tmp = tmp->next;
	}
	return (cmds);
}

char	parsing_error_check(t_token *tokens, t_macro *macro)
{
	char	c;
	t_token	*tmp;

	c = 0;
	tmp = tokens;
	if (tmp && tmp->type == PIPE)
		c = '|';
	while (tmp && tmp->next)
	{
		if (tmp->type == PIPE && tmp->next->type == PIPE)
			c = '|';
		tmp = tmp->next;
	}
	if (tmp && tmp->type == PIPE)
		c = '|';
	if (c != 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		macro->exit_code = 2;
	}
	return (c);
}
