/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:22:03 by gansari           #+#    #+#             */
/*   Updated: 2025/05/22 21:21:56 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * handle_builtin - Executes a shell built-in command and 
 * exits the process
 *
 * This function handles the execution of shell built-in commands such as
 * cd, echo, pwd, export, unset, env, and exit. It calls the appropriate
 * builtin function based on the command name found in ecmd->argv[0].
 *
 * The exec_builtin function returns an exit code indicating success (0)
 * or failure (non-zero). This exit code is then used to exit the process
 * with the appropriate status, allowing the parent process to determine
 * if the built-in command succeeded or failed.
 *
 * @param ecmd   Pointer to the exec command structure 
 * containing command and args
 * @param shell  Pointer to the shell structure with environment and state
 *
 * Note: This function does not return - it always exits the process.
 */

static void	handle_builtin(t_execcmd *ecmd, t_shell *shell)
{
	int	exit_code;

	exit_code = exec_builtin(ecmd, shell);
	exit(exit_code);
}

/**
 * check_if_directory - Checks if the path is a directory
 *
 * Uses stat() to check if the given path exists and is a directory.
 * This helps provide the correct error message when trying to execute
 * a directory path.
 *
 * @param path  The path to check
 * @return      1 if it's a directory, 0 otherwise
 */
static int	check_if_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
		return (S_ISDIR(path_stat.st_mode));
	return (0);
}

/**
 * command_not_found - Handles the case where a command doesn't exist
 *
 * This function displays an error message when a command can't be found in PATH
 * or as a built-in. It prints the standard shell error message format to stderr,
 * then exits the process with status 127, which is the conventional exit code
 * for "command not found" in Unix shells.
 *
 * @param cmd  The command name that was not found
 *
 * Note: This function does not return - it always exits the process.
 */
static void	command_not_found(char *cmd)
{
	ft_putstr_fd("sadaf: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

/**
 * handle_directory_error - Handles when trying to execute a directory
 *
 * This function provides the correct error message when a user tries
 * to execute a directory (like $HOME). It prints "Is a directory" error
 * and exits with code 126.
 *
 * @param cmd  The directory path that was attempted to execute
 *
 * Note: This function does not return - it always exits the process.
 */
static void	handle_directory_error(char *cmd)
{
	ft_putstr_fd("sadaf: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	exit(126);
}

static char	**prepare_unquoted_args(char **argv, char *path)
{
	char	**unquoted_argv;
	int		i;

	unquoted_argv = allocate_unquoted_array(argv, path);
	i = 0;
	while (argv[i])
	{
		unquoted_argv[i] = remove_quotes(argv[i]);
		if (!unquoted_argv[i])
			handle_unquote_error(unquoted_argv, path);
		i++;
	}
	unquoted_argv[i] = NULL;
	return (unquoted_argv);
}

/**
 * exec_external_command - Executes an external (non-built-in) command
 *
 * This function is responsible for executing external commands by:
 * 1. Converting the shell's environment linked list to a string array format
 *    compatible with execve()
 * 2. Calling execve() with the command path, arguments, and environment
 * 3. Cleaning up resources if execve() fails
 * 4. Reporting errors and exiting with appropriate status
 *
 * If execve() succeeds, it replaces the current process, so the function
 * only returns if an error occurred.
 *
 * @param path   Full path to the executable
 * @param argv   Argument array (including command as argv[0])
 * @param shell  Shell structure with environment variables
 *
 * Note: This function does not return on success - it either replaces
 * the process or exits with error status on failure.
 */
static void	exec_external_command(char *path, char **argv, t_shell *shell)
{
	char	**env_array;
	char	**unquoted_argv;

	unquoted_argv = prepare_unquoted_args(argv, path);
	env_array = env_to_array(shell->env_list);
	if (!env_array)
	{
		free(path);
		cleanup_tokens(unquoted_argv);
		ft_perror("env_to_array");
		exit(1);
	}
	if (execve(path, unquoted_argv, env_array) < 0)
	{
		free(path);
		cleanup_tokens(unquoted_argv);
		cleanup_tokens(env_array);
		ft_perror("execve");
		exit(1);
	}
}

/**
 * try_execute_as_command - Attempts to execute expanded variable as command
 *
 * When a variable expands to something like "ls -l", this function
 * tries to parse and execute it as a shell command.
 *
 * @param expanded_cmd  The expanded command string
 * @param shell         Shell structure
 */
static void	try_execute_as_command(char *expanded_cmd, t_shell *shell)
{
	char	**tokens;
	char	*path;
	int		i;

	// Split the expanded command into tokens
	tokens = ft_split(expanded_cmd, ' ');
	if (!tokens || !tokens[0])
	{
		if (tokens)
			cleanup_tokens(tokens);
		command_not_found(expanded_cmd);
		return;
	}

	// Check if first token is a builtin
	if (is_builtin(tokens[0]))
	{
		t_execcmd	cmd;
		int			status;

		ft_memset(&cmd, 0, sizeof(cmd));
		cmd.type = EXEC;
		i = 0;
		while (tokens[i] && i < MAXARGS - 1)
		{
			cmd.argv[i] = tokens[i];
			i++;
		}
		cmd.argv[i] = NULL;
		status = exec_builtin(&cmd, shell);
		cleanup_tokens(tokens);
		exit(status);
	}

	// Try to find the command in PATH
	path = find_command_path(tokens[0], shell);
	if (!path)
	{
		// Check if it's a directory
		if (check_if_directory(tokens[0]))
		{
			cleanup_tokens(tokens);
			handle_directory_error(tokens[0]);
		}
		cleanup_tokens(tokens);
		command_not_found(tokens[0]);
	}

	// Execute the external command
	exec_external_command(path, tokens, shell);
}

/**
 * execute_command - Main function for executing a command
 *
 * This is the primary function that handles command execution. It:
 * 1. Handles empty commands (just exit with success)
 * 2. Expands any environment variables in the command arguments
 * 3. Checks if the command is a shell built-in, and handles it if so
 * 4. Otherwise, searches for the command in PATH
 * 5. Reports an error if the command is not found
 * 6. Executes the external command if found
 *
 * This function serves as the coordinator between the various command
 * execution paths (built-ins vs. external commands) and handles the
 * preparation necessary before executing commands.
 *
 * @param ecmd   Execution command structure with command and args
 * @param shell  Shell structure with environment and state
 *
 * Note: This function does not return - it either:
 * - Exits with status 0 for empty commands
 * - Calls handle_builtin() which exits
 * - Calls command_not_found() which exits
 * - Calls exec_external_command() which replaces the process or exits
 */
void	execute_command(t_execcmd *ecmd, t_shell *shell)
{
	char	*path;
	char	*cmd_no_quotes;

	check_cmd_args(ecmd, shell);
	expand_variables(ecmd, shell);
	cmd_no_quotes = remove_quotes(ecmd->argv[0]);
	if (!cmd_no_quotes)
	{
		ft_error("remove_quotes failed");
		exit(1);
	}

	// Check if the expanded command contains spaces (indicating it might be a command with args)
	if (ft_strchr(cmd_no_quotes, ' '))
	{
		try_execute_as_command(cmd_no_quotes, shell);
		free(cmd_no_quotes);
		return;
	}

	if (is_builtin(cmd_no_quotes))
	{
		free(cmd_no_quotes);
		handle_builtin(ecmd, shell);
		return ;
	}

	// Check if it's a directory before trying to find path
	if (check_if_directory(cmd_no_quotes))
	{
		handle_directory_error(cmd_no_quotes);
		free(cmd_no_quotes);
		return;
	}

	path = find_command_path(cmd_no_quotes, shell);
	free(cmd_no_quotes);
	if (!path)
		command_not_found(ecmd->argv[0]);
	exec_external_command(path, ecmd->argv, shell);
	ft_error("execute_command: unreachable code");
	exit(1);
}
