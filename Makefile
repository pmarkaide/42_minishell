# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbejar-s <dbejar-s@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/01 15:11:43 by pmarkaid          #+#    #+#              #
#    Updated: 2024/08/09 20:10:58 by pmarkaid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_FILES = \
	main.c \
	presyntax.c \
	tokenizer.c \
	list_utils.c \
	print_utils.c \
	tokenizer_utils.c \
	free.c \

	parsing.c	\
	builtins.c	\
	env.c 	\
	main_utils.c 	\
	signals.c 	\
	bu_func.c 	\
  validation.c \
	execution.c \
	execution_utils.c

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

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

# External Libraries
LIBS = -lreadline

all: makelibft $(NAME)

makelibft:
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) $(LIBFT_INCLUDE) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_INCLUDE) $(INCLUDE) $(LIBFT) -o $(NAME) $(LIBS)

tests: makelibft $(TEST_OBJS) $(filter-out $(SRC_DIR)main.o, $(OBJS))
	$(CC) $(CFLAGS) $(LIBS) $(TEST_OBJS) $(filter-out $(SRC_DIR)main.o, $(OBJS)) $(LIBFT_INCLUDE) $(INCLUDE) $(LIBFT) -o $(TEST_NAME)
	./$(TEST_NAME)

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(TEST_OBJS)

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)
	rm -f $(TEST_NAME)

re: fclean all

.PHONY: all makelibft clean fclean re tests