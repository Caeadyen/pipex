# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hrings <hrings@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/15 15:57:35 by hrings            #+#    #+#              #
#    Updated: 2022/04/20 10:50:13 by hrings           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = pipex

CC = gcc

CFLAGS = -Wall -Werror -Wextra -O3
LIBRARIES = $(FT_LNK)
INCLUDES = $(FT_INC)
HEADER = ./header
SRCDIR = src/

LIBFT = $(LIBFT_DIRECTORY)/libft.a
LIBFT_DIRECTORY = ./libft
FT_INC	= -I ./libft/
FT_LNK	= ./libft/libft.a


OBJDIR = build/

SRCS := main.c utilities.c init.c error.c ft_split.c

SRCSB := main_bonus.c utilities_bonus.c init_bonus.c \
			error_bonus.c ft_split_bonus.c utilities2_bonus.c

OBJECTS := $(SRCS:.c=.o)
OBJECTS_PREFIXED := $(addprefix $(OBJDIR), $(OBJECTS))

OBJSB := $(SRCSB:.c=.o)
OBJECTSB_PREFIXED := $(addprefix $(OBJDIR), $(OBJSB))


GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

.PHONY: all clean fclean re rebonus bonus

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS_PREFIXED) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS_PREFIXED) $(LIBRARIES) -I$(HEADER) -o $(NAME)
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

$(OBJECTS_PREFIXED): build/%.o : src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c  $< -o $@ -I$(HEADER) $(INCLUDES)
	@echo "$(GREEN).$(RESET)\c"

$(OBJECTSB_PREFIXED): build/%.o : src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c  $< -o $@ -I$(HEADER) $(INCLUDES)
	@echo "$(GREEN).$(RESET)\c"
	
$(LIBFT):
	@echo "$(NAME): $(GREEN)Creating $(LIBFT)...$(RESET)"
	@$(MAKE) all -sC $(LIBFT_DIRECTORY)

bonus: $(LIBFT) $(OBJECTSB_PREFIXED) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTSB_PREFIXED) $(LIBRARIES) -I$(HEADER) -o $(NAME)
	@echo "$(NAME): $(GREEN)$(NAME) was created$(RESET)"

clean:
	@cd ./$(LIBFT_DIRECTORY) && make clean
	@rm -rf $(OBJDIR)
	@echo "$(NAME): $(RED)object files and directory were deleted$(RESET)"

fclean: clean
	@cd ./$(LIBFT_DIRECTORY) && make fclean
	@rm -rf $(OBJDIR)
	@rm -f $(NAME)
	@echo "$(NAME): $(RED)Libaries were deleted$(RESET)"

re: fclean all

rebonus: fclean bonus