/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:22:03 by gansari           #+#    #+#             */
/*   Updated: 2025/05/07 14:40:22 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * handle_builtin - Executes a shell built-in command and exits the process
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
 * @param ecmd   Pointer to the exec command structure containing command and args
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
	ft_putstr_fd("sadaf: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	exit(127);
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

	env_array = env_to_array(shell->env_list);
	if (!env_array)
	{
		free(path);
		ft_perror("env_to_array");
		exit(1);
	}
	if (execve(path, argv, env_array) < 0)
	{
		free(path);
		cleanup_tokens(env_array);
		ft_perror("execve");
		exit(1);
	}
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

	if (!ecmd || !shell)
	{
		ft_error("execute_command: NULL pointer");
		exit(1);
	}
	if (!ecmd->argv[0])
		exit(0);
	expand_variables(ecmd, shell);
	if (is_builtin(ecmd->argv[0]))
		handle_builtin(ecmd, shell);
	path = find_command_path(ecmd->argv[0], shell);
	if (!path)
		command_not_found(ecmd->argv[0]);
	exec_external_command(path, ecmd->argv, shell);
	ft_error("execute_command: unreachable code");
	exit(1);
}
