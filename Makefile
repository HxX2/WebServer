# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cipher <cipher@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/28 18:05:37 by zlafou            #+#    #+#              #
#    Updated: 2023/06/22 13:25:21 by cipher           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= webserv

SRCS			= main.cpp utils.cpp Config.cpp Server.cpp Request.cpp Response.cpp

OBJS			= $(SRCS:.cpp=.o)

CPPFLAGS		= -Wall -Wextra -Werror -std=c++98 -I ./includes #-g -fsanitize=address

CPP				= c++

VPATH 			= ./srcs

HEADER			= "\033[0;35m   █░░░█ █▀▀ █▀▀▄ █▀▀ █▀▀ █▀▀█ ▀█░█▀  \n   █▄█▄█ █▀▀ █▀▀▄ ▀▀█ █▀▀ █▄▄▀ ░█▄█░   \n   ░▀░▀░ ▀▀▀ ▀▀▀░ ▀▀▀ ▀▀▀ ▀░▀▀ ░░▀░░\033[0;0m"

FOOTER			= "\033[0;35m➔\033[0;0m type \033[0;34m./$(NAME)\033[0;0m to run the webserver."

vpath $(SRCS) $(VPATH)

all: $(NAME) | footer

header :
	@echo
	@echo $(HEADER)
	@echo

footer :
	@echo $(FOOTER)

.cpp.o: 
	@sleep 0.003
	@printf "\33[2K\r\033[1mmaking object \033[0;34m➔\033[0;0m\033[0;35m $@ \033[0;0m"
	-@$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME): | header $(OBJS)
	@echo
	@echo
	@echo "\033[0;35m\033[1m────── making webserv ──────\033[0;0m"
	@printf "\033[2m"
	$(CPP) ${OBJS} $(CPPFLAGS) -o ${NAME}
	@echo "\033[0;0"

clean: | header
	@echo "\033[0;35m\033[1m────── clean webserv ──────\033[0;0m"
	@printf "\033[2m"
	rm -rf $(OBJS)
	@echo "\033[0;0m"

fclean:	clean | header
	@echo "\033[0;35m\033[1m────── fclean webserv ──────\033[0;0m"
	@printf "\033[2m"
	rm -rf $(NAME)
	@echo "\033[0;0m"
				
re:	fclean all

.PHONY:	all clean fclean re
