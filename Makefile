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
SRC      = $(SRC_DIR)/sadaf.c\
		$(SRC_DIR)/builtins/builtin_cd.c $(SRC_DIR)/builtins/builtin_echo.c $(SRC_DIR)/builtins/builtin_echo_utils.c $(SRC_DIR)/builtins/builtin_echo_utils2.c\
		$(SRC_DIR)/builtins/builtin_env.c\
		$(SRC_DIR)/builtins/builtin_exit.c $(SRC_DIR)/builtins/builtin_export.c $(SRC_DIR)/builtins/builtin_export_utils.c\
		$(SRC_DIR)/builtins/builtin_pwd.c $(SRC_DIR)/builtins/builtin_unset.c $(SRC_DIR)/builtins/builtins.c\
		$(SRC_DIR)/cleaners/free_cmd.c $(SRC_DIR)/cleaners/free_envp.c\
		$(SRC_DIR)/env/parse_envp.c\
		$(SRC_DIR)/execution/collect_heredoc.c $(SRC_DIR)/execution/collect_heredoc_utils.c $(SRC_DIR)/execution/execute_command.c $(SRC_DIR)/execution/handle_background.c\
		$(SRC_DIR)/execution/handle_list.c $(SRC_DIR)/execution/handle_pipe.c $(SRC_DIR)/execution/handle_pipe_leftcmd.c $(SRC_DIR)/execution/execute_command1.c\
		$(SRC_DIR)/execution/handle_pipe_rightcmd.c $(SRC_DIR)/execution/handle_pipe_utils.c $(SRC_DIR)/execution/handle_redirections.c\
		$(SRC_DIR)/execution/handle_redirection_utils.c $(SRC_DIR)/execution/handle_redirection_utils2.c\
		$(SRC_DIR)/execution/heredoc.c $(SRC_DIR)/execution/path.c $(SRC_DIR)/execution/runcmd.c $(SRC_DIR)/execution/signals.c\
		$(SRC_DIR)/parsing/constructor.c $(SRC_DIR)/parsing/nullterminate.c $(SRC_DIR)/parsing/parse_cmd.c $(SRC_DIR)/parsing/parse_utils.c\
		$(SRC_DIR)/parsing/parseredir.c $(SRC_DIR)/parsing/parseredir_utils.c $(SRC_DIR)/parsing/parseblock.c $(SRC_DIR)/parsing/setting_prompt.c\
		$(SRC_DIR)/parsing/parseexec.c $(SRC_DIR)/parsing/token_utils1.c $(SRC_DIR)/parsing/token_utils2.c\
		$(SRC_DIR)/utils/env_to_array.c $(SRC_DIR)/utils/execute_command_utils.c $(SRC_DIR)/utils/execute_command_utils2.c $(SRC_DIR)/utils/execute_command_utils3.c $(SRC_DIR)/utils/export_utils.c\
		$(SRC_DIR)/utils/sadaf_utils.c $(SRC_DIR)/utils/env_utils.c	$(SRC_DIR)/utils/exec_utils.c $(SRC_DIR)/utils/ft_getenv.c\
		$(SRC_DIR)/utils/utils.c $(SRC_DIR)/utils/error_exit.c $(SRC_DIR)/utils/expand_var.c $(SRC_DIR)/utils/expand_var1.c $(SRC_DIR)/utils/expand_var2.c $(SRC_DIR)/utils/expand_var3.c\
		$(SRC_DIR)/utils/print_cmd.c $(SRC_DIR)/utils/utils2.c
OBJ_DIR  = ./obj
OBJ      = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
NAME     = minishell

# Default rule
all: $(LIBFT) $(NAME)

# Compile the program
$(NAME): $(OBJ) $(LIBFT)
	@echo "$(CYAN)🔨  Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline
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
valgrind: $(NAME)
	@echo "$(CYAN)🔍  Running Valgrind...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline_suppress.supp ./$(NAME)
	@echo "$(GREEN)✅  Valgrind run complete!$(RESET)"

valchild: $(NAME)
	@echo "$(CYAN)🔍  Running Valgrind with child tracing...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes --suppressions=readline_suppress.supp ./$(NAME)
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
	@rm -f $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✅  Executable and library deleted!$(RESET)"

# Rebuild everything
re: fclean all
	@echo "$(CYAN)🔄  Rebuilding everything...$(RESET)"

# Phony targets
.PHONY: all clean fclean re valgrind valchild
