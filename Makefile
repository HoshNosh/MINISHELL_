# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/25 13:01:19 by sdossa            #+#    #+#              #
#    Updated: 2025/12/06 13:40:55 by sdossa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#****************************************************************#
#                             NAMES                              #
#****************************************************************#

NAME 		= minishell

#****************************************************************#
#                     COMPILATION AND OPTIONS                    #
#****************************************************************#

LIBFT 		= ./libft/libft.a
LIBS 		= -lreadline

CC 			= cc
CFLAGS 		= -Wall -Wextra -Werror $(INCLUDES) -g
RM 			= rm -f
MKDIR 		= mkdir -p
INCLUDES	= -I includes -I libft

#****************************************************************#
#                          SOURCE FILES                          #
#****************************************************************#

SRCS_DIR	= srcs
SRCS 		= $(shell find $(SRCS_DIR) -name "*.c")


#****************************************************************#
#                          OBJECT FILES                          #
#****************************************************************#

OBJ_DIR 	= obj
OBJS 		= $(SRCS:$(SRCS_DIR)/%.c=$(OBJ_DIR)/%.o)


#****************************************************************#
#                               RULES                            #
#****************************************************************#

GREEN			= \033[32m
YELLOW			= \033[33m
RED				= \033[31m
BLUE			= \033[34m
MAGENTA			= \033[35m
CYAN			= \033[36m
BLACK			= \033[30m
WHITE			= \033[37m
BRIGHT_BLACK 	= \033[90m
BRIGHT_RED		= \033[91m
BRIGHT_GREEN	= \033[92m
BRIGHT_YELLOW	= \033[93m
BRIGHT_BLUE		= \033[94m
BRIGHT_MAGENTA	= \033[95m
BRIGHT_CYAN		= \033[96m
BRIGHT_WHITE	= \033[97m
VIOLET			= \033[38;5;129m
INDIGO			= \033[38;5;54m
ORANGE			= \033[38;5;208m
MARRON			= \033[38;5;94m
PRUNE			= \033[38;5;90m
BEIGE			= \033[38;5;229m
BOLD			= \033[1m
UNDERLINE		= \033[4m
RESET			= \033[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(BOLD)$(BRIGHT_YELLOW)Linking $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(BOLD)$(BRIGHT_GREEN)✅ $(NAME) created successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@echo "$(BOLD)$(BRIGHT_MAGENTA)Compiling $<...$(RESET)"
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "$(BOLD)$(VIOLET)Compiling libft...$(RESET)"
	make -C ./libft bonus
	@echo "$(BOLD)$(BRIGHT_GREEN)✅ libft compiled successfully!$(RESET)"

#****************************************************************#
#                         CLEANING RULES                         #
#****************************************************************#

clean:
		@echo "$(BOLD)$(ORANGE)Cleaning object files...$(RESET)"
		$(RM) -r $(OBJ_DIR)
		make clean -C ./libft

fclean: clean
		@echo "$(BOLD)$(YELLOW)Cleaning $(NAME)...$(RESET)"
		$(RM) $(NAME)
		make fclean -C ./libft


re:
	$(MAKE) fclean
	$(MAKE) all

norm:
	@echo "$(BOLD)$(BEIGE)Checking norminette...$(RESET)"
	@norminette $(SRCS_DIR) includes

.PHONY: all clean fclean re

