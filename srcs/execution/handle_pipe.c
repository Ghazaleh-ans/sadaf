/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:35:21 by gansari           #+#    #+#             */
/*   Updated: 2025/05/09 13:56:19 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * close_pipe_fds - Safely closes both pipe file descriptors
 *
 * This utility function closes both ends of a pipe (read and write)
 * and provides proper error handling. When setting up redirections for
 * pipe commands, we need to close the original pipe file descriptors
 * after duplicating them to avoid resource leaks and unexpected behavior.
 * This function centralizes that operation to ensure consistent error
 * handling across the codebase.
 *
 * @param fd  Array containing pipe file descriptors [read_end, write_end]
 * @return    0 on success, -1 on error if either descriptor fails to close
 */
static int	close_pipe_fds(int *fd)
{
	if (close_and_report(fd[0]) < 0 || close_and_report(fd[1]) < 0)
		return (-1);
	return (0);
}

/**
 * setup_pipe_input - Configures process to read from a pipe
 *
 * This function sets up a child process to read from the read-end of a pipe.
 * It performs three critical operations in sequence:
 * 1. Closes file descriptor 0 (standard input)
 * 2. Duplicates the read-end of the pipe (fd[0]) to file descriptor 0,
 *    which effectively redirects standard input to come from the pipe
 * 3. Closes both original pipe file descriptors to prevent resource leaks
 *
 * This function is specifically used for the right-side command in a pipe
 * expression (cmd2 in "cmd1 | cmd2") to make it read input from the output
 * of the left-side command. Any failure in this redirection setup causes
 * the function to return an error code.
 *
 * @param fd  Array containing pipe file descriptors [read_end, write_end]
 * @return    0 on success, -1 on error
 */
int	setup_pipe_input(int *fd)
{
	if (close_and_report(0) < 0)
		return (-1);
	if (dup_and_report(fd[0]) < 0)
		return (-1);
	if (close_pipe_fds(fd) < 0)
		return (-1);
	return (0);
}

/**
 * setup_pipe_output - Configures process to write to a pipe
 *
 * This function sets up a child process to write to the write-end of a pipe.
 * It performs a series of critical operations in sequence:
 * 1. Closes file descriptor 1 (standard output)
 * 2. Duplicates the write-end of the pipe (fd[1]) to file descriptor 1,
 *    which effectively redirects any output sent to stdout to go into the pipe
 * 3. Closes both original pipe file descriptors to prevent resource leaks
 *
 * This function is specifically used for the left-side command in a pipe
 * expression (cmd1 in "cmd1 | cmd2") to make its output go to the input of
 * the right-side command. Any failure in the redirection setup causes the
 * function to return an error code, allowing the caller to handle the error
 * appropriately.
 *
 * @param fd  Array containing pipe file descriptors [read_end, write_end]
 * @return    0 on success, -1 on error
 */
int	setup_pipe_output(int *fd)
{
	if (close_and_report(1) < 0)
		return (-1);
	if (dup_and_report(fd[1]) < 0)
		return (-1);
	if (close_pipe_fds(fd) < 0)
		return (-1);
	return (0);
}

/**
 * wait_for_children - Waits for child processes and captures exit status
 *
 * This function waits for both child processes created during pipe execution
 * to complete and captures the exit status of the right-side command. It performs
 * these operations:
 * 1. Waits for the left command process (pid1) to exit, discarding its status
 * 2. Waits for the right command process (pid2) to exit, capturing its status
 * 3. Interprets the exit status and sets shell->exit_status accordingly:
 *    - For normal exits (WIFEXITED), it stores the exit code (WEXITSTATUS)
 *    - For signal terminations (WIFSIGNALED), it stores 128 + signal number
 *
 * This follows the shell convention where the exit status of a pipeline is
 * the exit status of the last command in the pipeline. The function ensures
 * both processes are properly waited for to prevent zombie processes, even though
 * only the right command's exit status is used. The exit status is stored in the
 * shell structure for later use by the shell (e.g., for the $? variable).
 *
 * @param pid1   PID of the left command process
 * @param pid2   PID of the right command process
 * @param shell  Pointer to shell structure to store exit status
 */
static void	wait_for_children(int pid1, int pid2, t_shell *shell)
{
	int	status;

	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

/**
 * handle_pipe - Main function to execute piped commands
 *
 * This function handles the execution of a pipeline (e.g., "cmd1 | cmd2").
 * It orchestrates the entire pipe execution process and follows these steps:
 * 1. Creates a pipe using the create_pipe() function
 * 2. Sets up signal handling for child processes using setup_signals(1)
 * 3. Creates and executes the left command process using execute_left_cmd()
 * 4. Creates and executes the right command process using execute_right_cmd()
 * 5. Closes the pipe file descriptors in the parent process using safe_close()
 * 6. Waits for both child processes to complete using wait_for_children()
 * 7. Restores normal signal handling using setup_signals(0)
 *
 * The function includes comprehensive error handling at each step. If any
 * operation fails (pipe creation, process creation), it performs appropriate
 * cleanup using the cleanup_pipe() function and exits with a failure status.
 *
 * For complex pipe chains (e.g., cmd1 | cmd2 | cmd3), the recursive
 * nature of the parser and executor handles them correctly as nested
 * pipe commands, where the right command of one pipe can itself be another
 * pipe command. This allows the shell to support pipelines of arbitrary length.
 *
 * @param pcmd   Pointer to pipe command structure containing left and right commands
 * @param shell  Pointer to shell structure with environment variables and state
 */
void	handle_pipe(t_pipecmd *pcmd, t_shell *shell)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (create_pipe(fd) < 0)
		exit(EXIT_FAILURE);
	setup_signals(1);
	pid1 = execute_left_cmd(pcmd, shell, fd);
	if (pid1 < 0)
	{
		cleanup_pipe(fd, -1);
		exit(EXIT_FAILURE);
	}
	pid2 = execute_right_cmd(pcmd, shell, fd);
	if (pid2 < 0)
	{
		cleanup_pipe(fd, pid1);
		exit(EXIT_FAILURE);
	}
	safe_close(fd[0]);
	safe_close(fd[1]);
	wait_for_children(pid1, pid2, shell);
	setup_signals(0);
}
