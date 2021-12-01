# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/21 19:53:07 by oidrissi          #+#    #+#              #
#    Updated: 2021/11/26 12:05:34 by oidrissi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
LDFLAGS= -L /Users/$(USER)/goinfre/$(USER)/.brew/opt/readline/lib
CPPFLAGS= -I /Users/$(USER)/goinfre/$(USER)/.brew/opt/readline/include
READLINE = -lreadline $(LDFLAGS) $(CPPFLAGS)
FLAGS = $(READLINE) -fsanitize=address -g
NAME = hell420

SRC = minishell.c\
		utils.c\
		utils2.c\
		utils3.c\
		utils4.c\
		methods.c\
		expansion.c\
		expansion2.c\
		expansion3.c\
		sigs.c\
		redir.c\
		init.c\


all: $(NAME)

$(NAME): $(SRC)
	@$(CC) $(SRC) $(FLAGS) -o $(NAME)

clean:
	@rm -rf $(NAME)

fclean: clean

re: fclean all