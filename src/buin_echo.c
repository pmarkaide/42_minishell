/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buin_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:04:32 by dbejar-s          #+#    #+#             */
/*   Updated: 2024/09/04 18:11:39 by dbejar-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_n(char **args, int *i, int *n_flag)
{
	int	j;

	*i = 1;
	*n_flag = 0;
	while (args[*i] && args[*i][0] == '-' && args[*i][1] == 'n')
	{
		j = 2;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j] == '\0')
		{
			*n_flag = 1;
			(*i)++;
		}
		else
			break ;
	}
}

static void	print_echo(char **args, int i, int n_flag)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

int	ft_echo2(char **args)
{
	int	i;
	int	n_flag;

	check_n(args, &i, &n_flag);
	print_echo(args, i, n_flag);
	return (0);
}
