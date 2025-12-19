# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nadgalle <nadgalle@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/28 19:36:45 by sdossa            #+#    #+#              #
#    Updated: 2025/12/08 17:15:22 by nadgalle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re bonus

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(HEADER)
HEADER = .

BONUS = ft_bonus.lstadd_back.c ft_bonus.lstadd_front.c ft_bonus.lstclear.c \
        ft_bonus.lstdelone.c ft_bonus.lstiter.c ft_bonus.lstlast.c \
        ft_bonus.lstmap.c ft_bonus.lstnew.c ft_bonus.lstsize.c

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_realloc.c\
ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_strcmp.c ft_strncmp.c ft_isnumber.c ft_free_arr.c get_next_line.c get_next_line_utils.c

OBJ = $(SRCS:.c=.o)
OBJB = $(BONUS:.c=.o)


GREEN		= \033[32m
YELLOW		= \033[33m
RED			= \033[31m
BLUE		= \033[34m
MAGENTA		= \033[35m
CYAN		= \033[36m
VIOLET		= \033[38;5;129m
INDIGO		= \033[38;5;54m
ORANGE		= \033[38;5;208m
MARRON		= \033[38;5;94m
PRUNE		= \033[38;5;90m
BEIGE		= \033[38;5;229m
BOLD		= \033[1m
UNDERLINE	= \033[4m
RESET		= \033[0m

all: $(NAME) bonus

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)

%.o: %.c
	@echo "$(BOLD)$(VIOLET)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -o $@ -c $<

bonus: $(NAME) $(OBJB)
	@ar rcs -o $(NAME) $(OBJB)

clean:
	@rm -f $(OBJ) $(OBJB)

fclean: clean
	@rm -f $(NAME)

re: fclean all
