/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:42:50 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/28 17:58:55 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fix_redirections(char *ins)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (ins[i] != '\0')
	{
		if ((ins[i] == '>' || ins[i] == '<') && !is_in_quote(ins, i))
		{
			ins[j++] = ins[i];
			i++;
			while (ft_isspace(ins[i]))
				i++;
			i--;
		}
		else
			ins[j++] = ins[i];
		i++;
	}
	ins[j] = '\0';
}

bool	is_redir(t_token *token, char *redir_type)
{
	if (!token || !redir_type)
		return (false);
	if (ft_strcmp(redir_type, "input") == 0)
	{
		if (token->type == INRED || token->type == HERE_DOC)
			return (true);
	}
	else if (ft_strcmp(redir_type, "output") == 0)
	{
		if (token->type == OUTRED || token->type == APPEND)
			return (true);
	}
	return (false);
}

bool	is_builtin(t_token *token)
{
	if (ft_strcmp(token->value, "echo") == 0)
		return (true);
	if (ft_strcmp(token->value, "cd") == 0)
		return (true);
	if (ft_strcmp(token->value, "pwd") == 0)
		return (true);
	if (ft_strcmp(token->value, "export") == 0)
		return (true);
	if (ft_strcmp(token->value, "unset") == 0)
		return (true);
	if (ft_strcmp(token->value, "env") == 0)
		return (true);
	if (ft_strcmp(token->value, "exit") == 0)
		return (true);
	return (false);
}

char	*clean_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	bool	double_quote_open;
	bool	single_quote_open;

	result = (char *)malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	double_quote_open = false;
	single_quote_open = false;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		else if (str[i] == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	free(str);
	return (result);
}

void	clean_token_quotes(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type != HERE_DOC)
			tmp->value = clean_quotes(tmp->value);
		tmp = tmp->next;
	}
}
