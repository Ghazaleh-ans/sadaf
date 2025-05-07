# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 18:31:20 by mukibrok          #+#    #+#              #
#    Updated: 2025/04/24 17:55:17 by mukibrok         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Color definitions
RESET   = \033[0m
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
CYAN    = \033[36m

# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
RM      = rm -rf
AR      = ar rcs
RANLIB  = ranlib

# Executable name
NAME    = libft.a

# Files
SRC     = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
          ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c \
          ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
          ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strdup.c ft_strchr.c \
          ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strmapi.c ft_strnstr.c \
          ft_strrchr.c ft_strtrim.c ft_strlen.c ft_strncmp.c ft_substr.c \
          ft_tolower.c ft_toupper.c ft_itoa.c ft_striteri.c ft_strcmp.c

BONUS_SRC = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
            ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstmap.c \
            ft_lstiter.c

# Object files
OBJ         = $(SRC:.c=.o)
BONUS_OBJ   = $(BONUS_SRC:.c=.o)

# Rules
all: $(NAME)

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Silent compilation of object files
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@


# Target for creating the static library with progress messages
$(NAME): $(OBJ)
	@echo "$(CYAN)📚  Building $(NAME)...$(RESET)"
	@$(AR) $(NAME) $(OBJ)
	@$(RANLIB) $(NAME)
	@echo "$(GREEN)✅  $(NAME) created successfully!$(RESET)"

# Bonus target with progress messages
bonus: $(OBJ) $(BONUS_OBJ)
	@echo "$(CYAN)✨  Building bonus objects...$(RESET)"
	@$(AR) $(NAME) $(OBJ) $(BONUS_OBJ)
	@$(RANLIB) $(NAME)
	@echo "$(GREEN)✅  Bonus added to $(NAME) successfully!$(RESET)"

# Clean targets with messages
clean:
	@echo "$(RED)🧹  Cleaning object files...$(RESET)"
	@$(RM) $(OBJ) $(BONUS_OBJ)
	@echo "$(GREEN)✅  Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(RED)🗑️   Removing $(NAME)...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)✅  $(NAME) removed!$(RESET)"

re: fclean all
	@echo "$(CYAN)🔄  Project rebuilt!$(RESET)"

.PHONY: all clean fclean re bonus