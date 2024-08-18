/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:07:27 by pmarkaid          #+#    #+#             */
/*   Updated: 2024/08/16 15:50:14 by pmarkaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char *skip_quotes(char *str, char quote) {
    while (*str && *str != quote) {
        str++;
    }
    if (*str == quote) {
        str++; // Move past the closing quote
    }
    return str;
}

void remove_empty_quotes(char *str) {
    char *write_ptr = str;

    while (*str) {
        printf("Processing: %c\n", *str);  // Debug print
        if ((*str == '"' && *(str + 1) == '"') || 
            (*str == '\'' && *(str + 1) == '\'')) {
            str += 2;  // Skip the empty quotes
        } else {
            *write_ptr++ = *str++;
        }
    }
    *write_ptr = '\0';  // Properly terminate the modified string
}


int count_tokens(char *str) {
    int count = 0;
    int inside_quotes = 0;
    char quote_char = 0;
    int in_token = 0;

    while (*str) {
        if (*str == '"' || *str == '\'') {
            if (!inside_quotes) {
                inside_quotes = 1;
                quote_char = *str;
                if (!in_token) {
                    count++;
                    in_token = 1;
                }
            } else if (*str == quote_char) {
                inside_quotes = 0;
            }
        } else if (*str == ' ') {
            if (!inside_quotes && in_token) {
                in_token = 0;
            }
        } else {
            if (!in_token) {
                count++;
                in_token = 1;
            }
        }
        str++;
    }

    return count;
}




void write_token(char *token, char *start, int length, char quote) {
    if (quote) {
        *token++ = quote; // Add opening quote
    }
    while (length--) {
        *token++ = *start++;
    }
    *token = '\0'; // Null-terminate the string
}


char *extract_token(char *str, int *length) {
    char *start;
    char quote = '\0';
    char *token;
    int extra_length = 0;

    start = str;

    if (ft_isquote(*str)) {
        quote = *str;
        start++;
        str = skip_quotes(start, quote);
        *length = str - start;
        extra_length = 2; // For the quotes
    } else {
        while (*str && !ft_isspace((unsigned char)*str) && !ft_isquote(*str)) {
            str++;
        }
        *length = str - start;
    }

    // Add 1 to account for null terminator
    token = (char *)malloc((*length + extra_length + 1) * sizeof(char));
    if (token == NULL) {
        return NULL;
    }

    write_token(token, start, *length, quote);
    return token;
}



int main(void) {
    char str1[] = "echo '' \"\" plain_argument";
    char str2[] = "echo hello world";
    
    printf("Before: %s\n", str1);
    remove_empty_quotes(str1);
    printf("After: %s\n\n", str1);

    printf("Before: %s\n", str2);
    remove_empty_quotes(str2);
    printf("After: %s\n\n", str2);

    return 0;
}



// int main() {
//     char *input = "cat \"infile 'this is' a test\" another quoted string plain_argument";
//     char *token;
//     int length;

// 	int count = count_tokens(input);
// 	printf("Token count: %d\n", count);
// 	return (1);

//     while (*input) {
//         while (ft_isspace((unsigned char)*input)) {
//             input++;
//         }
//         if (*input == '\0') {
//             break;
//         }
//         token = extract_token(input, &length);
//         if (token == NULL) {
//             fprintf(stderr, "Memory allocation failed\n");
//             return 1;
//         }
//         printf("Extracted token: %s\n", token);
//         free(token);
//         input += length;
//     }

//     return 0;
// }
