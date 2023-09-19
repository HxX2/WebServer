CC			= c++

NAME		= webserv

FLAGS		= -Wall -Wextra -Werror -std=c++98 -I ./includes

SRCS_DIR	= srcs
SRCS		= Config.cpp \
			  Config_Location.cpp \
			  Config_Server.cpp \
			  main.cpp \
			  Client.cpp \
			  Server.cpp \
			  ServersManager.cpp \
			  utils.cpp \
			  Templates.cpp \
			  

OBJ_DIR		= obj
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(FLAGS) -o $(NAME)

debug: $(OBJS)
	$(CC) $(OBJS) $(FLAGS) -D __DEBUG=true -g3 -fsanitize=address -o $(NAME)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
