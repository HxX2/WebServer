CC			= c++

NAME		= webserv

FLAGS		= -Wall -Wextra -Werror -std=c++98 -I ./includes

SRCS_DIR	= srcs
SRCS		= Config.cpp Logger.cpp main.cpp Request.cpp Response.cpp Server.cpp ServersManager.cpp utils.cpp

OBJ_DIR		= obj
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(FLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
