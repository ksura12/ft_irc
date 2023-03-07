# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/05 14:19:51 by ksura             #+#    #+#              #
#    Updated: 2023/03/05 14:20:08 by ksura            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC			= c++
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -ggdb3 -fsanitize=address -Wpedantic
RM			= rm -f
NAME		= bot

SRC			= 	main.cpp \

CFLAGS := -Wall -Werror -Wextra -std=c++98
HEADER = ./header
OBJDIR = build/


GREEN = \033[0;32m
RED = \033[0;31m
COLOR_DEFAULT = \033[0m

OBJS = ${SRC:.cpp=.o}

OBJECTS_PREF := $(addprefix $(OBJDIR), $(OBJS))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJECTS_PREF) $(OBJDIR)
	@$(CC) $(CFLAGS) $(OBJECTS_PREF) -I$(HEADER) -o $@
	@echo "$(GREEN)$(NAME) was created"

$(OBJECTS_PREF): build/%.o : srcs/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(HEADER) $(INCLUDES)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) was deleted"

re: fclean all