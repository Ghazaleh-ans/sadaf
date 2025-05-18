# Color definitions
RESET   = \033[0m
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
CYAN    = \033[36m

# Variables
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
LIBFT_DIR = ./includes/libft
LIBFT    = $(LIBFT_DIR)/libft.a
INCLUDES = -I./includes -I$(LIBFT_DIR)
SRC_DIR  = ./srcs
SRC      = $(wildcard $(SRC_DIR)/*.c) \
           $(wildcard $(SRC_DIR)/builtins/*.c) \
           $(wildcard $(SRC_DIR)/cleaners/*.c) \
           $(wildcard $(SRC_DIR)/execution/*.c) \
           $(wildcard $(SRC_DIR)/parsing/*.c) \
           $(wildcard $(SRC_DIR)/utils/*.c) \
           $(wildcard $(SRC_DIR)/env/*.c)
OBJ_DIR  = ./obj
OBJ      = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
EXEC     = sadaf

# Default rule
all: $(LIBFT) $(EXEC)

# Compile the program
$(EXEC): $(OBJ)
	@echo "$(CYAN)🔨  Compiling $(EXEC)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(EXEC) -lreadline
	@echo "$(GREEN)✅  Compilation successful!$(RESET)"

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(YELLOW)📂  Creating object file: $@$(RESET)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build libft library
$(LIBFT):
	@echo "$(CYAN)🔨  Building libft library...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/builtins
	@mkdir -p $(OBJ_DIR)/cleaners
	@mkdir -p $(OBJ_DIR)/execution
	@mkdir -p $(OBJ_DIR)/parsing
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/env

# Check memory leaks
valgrind: $(EXEC)
	@echo "$(CYAN)🔍  Running Valgrind...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline_suppress.supp ./$(EXEC)
	@echo "$(GREEN)✅  Valgrind run complete!$(RESET)"

valchild: $(EXEC)
	@echo "$(CYAN)🔍  Running Valgrind with child tracing...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes --suppressions=readline_suppress.supp ./$(EXEC)
	@echo "$(GREEN)✅  Valgrind with child tracing complete!$(RESET)"

# Clean up object files
clean:
	@echo "$(RED)🧹  Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@echo "$(GREEN)✅  Object files cleaned!$(RESET)"

# Clean up all generated files
fclean: clean
	@echo "$(RED)🗑️   Deleting executable and libft library...$(RESET)"
	@rm -f $(EXEC)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✅  Executable and library deleted!$(RESET)"

# Rebuild everything
re: fclean all
	@echo "$(CYAN)🔄  Rebuilding everything...$(RESET)"

# Phony targets
.PHONY: all clean fclean re valgrind valchild
