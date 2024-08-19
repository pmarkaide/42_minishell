/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_edge_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:33:28 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/19 13:48:20 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_remove_edge_quotes(char *str)
{
    int start = 0;
    int end;
    int len;
    char *result;

    if (str == NULL || *str == '\0')
        return NULL;
    len = ft_strlen(str);
    if (len == 0)
        return ft_strdup("");
    end = len - 1;
    while (str[start] == '\'' || str[start] == '\"')
        start++;
    while (end > start && (str[end] == '\'' || str[end] == '\"'))
        end--;
    result = malloc(end - start + 2);
    if (result == NULL)
        return(NULL);
    ft_strncpy(result, str + start, end - start + 1);
    result[end - start + 1] = '\0';
    return result;
}
