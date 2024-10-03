# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/21 14:23:25 by Helene            #+#    #+#              #
#    Updated: 2024/10/03 15:36:50 by hlesny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = g++ # c++ ? std::string incomplete type error when compiling using c++ 
CFLAGS = -std=c++98 -Wall -Wextra -g3 #-v (verbose) #-Werror -fsanitize=address
RM = rm -f

SRCS_DIR = ./sources/
OBJS_DIR = ./objects/
INCS_DIR = ./includes/

SRCS = 	main.cpp \
		Server.cpp \
		Server_pollins.cpp \
		Server_pollouts.cpp \
		Client.cpp \
		Channel.cpp \
		Logger.cpp \
		CommandsHandler.cpp \
		Commands/utils.cpp \
		Commands/pass.cpp \
		Commands/user.cpp \
		Commands/nick.cpp \
		Commands/motd.cpp \
		Commands/ping.cpp \
		Commands/quit.cpp \
		Commands/join.cpp 
		
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
DEPS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.d))

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.cpp
	mkdir -p $(OBJS_DIR)
	mkdir -p $(OBJS_DIR)Commands/
	$(CC) $(CFLAGS) -MMD -I $(INCS_DIR) -c $< -o $@ 

-include $(DEPS)

clean : 
	$(RM) -r $(OBJS_DIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY:
	all clean fclean re