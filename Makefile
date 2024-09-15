# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmarkaid <pmarkaid@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/01 15:11:43 by pmarkaid          #+#    #+#              #
#    Updated: 2024/09/15 21:04:10 by pmarkaid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_FILES =                              \
    buin_cd.c                             \
    buin_cd_utils.c                       \
    buin_echo.c                           \
    buin_env.c                            \
    buin_exit.c                           \
    buin_export.c                         \
    buin_export_utils.c                   \
    buin_pwd.c                            \
    buin_unset.c                          \
    dup.c                                 \
    env.c                                 \
    error.c                               \
    execution.c                           \
    execution_builtin.c                   \
    execution_utils.c                     \
    expand.c                              \
    expand_cases.c                        \
    expand_utils.c                        \
    free.c                                \
    free_utils.c                          \
    general_utils.c                       \
    list_cmd.c                            \
    list_token.c                          \
    main.c                                \
    main_path.c                           \
    main_utils.c                          \
    parsing.c                             \
    parsing_here_doc.c                    \
    parsing_utils.c                       \
    presyntax.c                           \
    signals.c                             \
    split_args.c                          \
    tokenizer.c                           \
    tokenizer_expand.c                    \
    tokenizer_retokenize.c                \
    tokenizer_utils.c                     \
    validation.c                          \
    validation_utils.c                    \
    zz_print_utils.c

SRC_DIR = src/
SRCS = $(addprefix $(SRC_DIR), $(SRCS_FILES))
OBJS = $(SRCS:.c=.o)

INCLUDE = -I ./include/

LIBFT_DIR = lib/libft/
LIBFT = $(LIBFT_DIR)libft.a
LIBFT_INCLUDE = -I $(LIBFT_DIR)

TEST_SRCS = $(wildcard tests/*.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_NAME = tests_runner

LIBS = -lreadline

CC = cc 
CFLAGS = -Wall -Werror -Wextra -g
DEBUG_FLAGS = -Wall -Werror -Wextra -fsanitize=address

LIBS = -lreadline

all: makelibft $(NAME)

makelibft:
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(LIBFT_INCLUDE) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_INCLUDE) $(INCLUDE) $(LIBFT) -o $(NAME) $(LIBS)

tests: makelibft $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(TEST_OBJS) $(filter-out src/main.o, $(OBJS)) $(LIBFT_INCLUDE) $(INCLUDE) $(LIBFT) -o $(TEST_NAME)
	./$(TEST_NAME)

debug: makelibft $(OBJS) $(LIBFT)
	$(CC) $(DEBUG_FLAGS) $(CFLAGS) $(OBJS) $(LIBFT_INCLUDE) $(INCLUDE) $(LIBFT) -o $(NAME) $(LIBS)

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(TEST_OBJS)

fclean: clean
	rm -f $(NAME) $(LIBFT) $(TEST_NAME)

re: fclean all

.PHONY: all makelibft clean fclean re debug tests
