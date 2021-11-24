# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/21 19:53:07 by oidrissi          #+#    #+#              #
#    Updated: 2021/11/23 16:06:52 by oidrissi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
FLAGS = -pthread -Wall -Wextra -Werror -lreadline -fsanitize=address -g
NAME = hell420

SRC = minishell.c\
		utils.c\

all: $(NAME)

$(NAME): $(SRC)
	@$(CC) $(SRC) $(FLAGS) -o $(NAME)

clean:
	@rm -rf $(NAME)

fclean: clean

re: fclean all