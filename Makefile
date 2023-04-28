# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/25 10:45:55 by iamongeo          #+#    #+#              #
#    Updated: 2023/04/25 16:18:36 by iamongeo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CLT_SRCS	:= client_socket_tcp.c 
CLT_SRCS	:= $(addprefix src/, $(CLT_SRCS))

SRV_SRCS	:= server_socket_tcp.c
SRV_SRCS	:= $(addprefix src/, $(SRV_SRCS))

CLT_OBJS	:= $(CLT_SRCS:.c=.o)
SRV_OBJS	:= $(SRV_SRCS:.c=.o)

#LIBFTDIR	:= libft/
#LIBFT		:= $(LIBFTDIR)libft.a

CFLAGS	:= -Wall -Werror -Wextra -g
CC	:= gcc

#INCL	:= $(LIBFTDIR)

CLT_NAME	:= client_endpoint
SRV_NAME	:= server_endpoint

%.o:	%.c
	@echo "compiling" $< "into" $@
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): all

$(LIBFT):
	make -C libft/

all:	$(LIBFT) $(CLT_OBJS) $(SRV_OBJS)
	$(CC) $(CFLAGS) -o $(CLT_NAME) $(CLT_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(SRV_NAME) $(SRV_OBJS) $(LIBFT)

clean:
	rm -f $(CLT_OBJS)
	rm -f $(SRV_OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
