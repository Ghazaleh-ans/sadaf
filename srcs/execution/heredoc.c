/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:39:03 by gansari           #+#    #+#             */
/*   Updated: 2025/05/07 13:11:54 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * setup_heredoc_pipe - Creates a pipe for heredoc data transfer
 * @fd: Array to store pipe file descriptors [0:read end, 1:write end]
 *
 * Purpose:
 * Creates a unidirectional data channel to transfer heredoc content
 * from the child (input collection) process to the parent process.
 *
 * Technical details:
 * - Uses the pipe() system call to create a pipe
 * - Handles errors by printing a message and returning error code
 * - fd[0]: read end of pipe, where data will be read from
 * - fd[1]: write end of pipe, where data will be written to
 *
 * Return values:
 * - 0 on success
 * - -1 on error (pipe creation failure)
 */
static int	setup_heredoc_pipe(int *fd)
{
	if (pipe(fd) == -1)
	{
		ft_perror("heredoc");
		return (-1);
	}
	return (0);
}

/**
 * child_heredoc_process - Manages heredoc input collection in a child process
 * @fd_write: File descriptor for writing collected input
 * @delimiter: String that marks the end of heredoc input
 * @shell: Shell state information
 *
 * Purpose:
 * This function runs in a forked child process to collect user input until
 * the delimiter is encountered, writing that input to the pipe.
 *
 * Technical details:
 * - Resets signal handling to default behavior (allows interruption with Ctrl+C)
 * - Sets the shell's heredoc flag to indicate heredoc input mode
 * - Calls process_heredoc_input to handle the actual input collection logic
 * - Closes the write end of the pipe when done
 * - Exits the child process with success status (0)
 *
 * Note:
 * This function never returns - it always exits the process when done
 */
static void	child_heredoc_process(int fd_write, char *delimiter, t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	shell->in_heredoc = 1;
	process_heredoc_input(fd_write, delimiter, shell);
	close(fd_write);
	exit(0);
}

/**
 * wait_for_heredoc_child - Waits for the heredoc child process to complete
 * @pid: Process ID of the child process to wait for
 * @fd_read: File descriptor for reading heredoc content
 *
 * Purpose:
 * Waits for heredoc input collection to complete and handles possible
 * termination by signal (e.g., user pressed Ctrl+C during input).
 *
 * Technical details:
 * - Uses waitpid() to wait specifically for the heredoc child process
 * - Checks if the process was terminated by a signal (WIFSIGNALED)
 * - Sets global signal flag if termination was due to a signal
 * - Closes the read pipe end if terminated by signal
 *
 * Return values:
 * - fd_read if child completed normally (input can be read)
 * - -1 if child was terminated by a signal (operation canceled)
 */
static int	wait_for_heredoc_child(int pid, int fd_read)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		g_signal_received = 1;
		close(fd_read);
		return (-1);
	}
	return (fd_read);
}

/**
 * handle_heredoc - Main entry point for heredoc handling
 * @delimiter: String that marks the end of heredoc input
 * @shell: Shell state information
 *
 * Purpose:
 * Coordinates the entire heredoc operation, from setup to completion.
 * Creates a pipe, forks a child process to collect input, and waits
 * for input collection to finish.
 *
 * Technical workflow:
 * 1. Sets up a pipe for communication between processes
 * 2. Forks a child process to handle input collection
 * 3. Parent closes write end of pipe (won't be writing)
 * 4. Parent waits for child to finish collecting input
 * 5. Returns read end of pipe or error code
 *
 * Return values:
 * - A valid file descriptor (>=0) for reading heredoc content on success
 * - -1 on error or if operation was interrupted
 *
 * Notes:
 * - The caller is responsible for reading from the returned file descriptor
 * - The caller is responsible for closing the file descriptor when done
 */
int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int	fd[2];
	int	pid;

	if (setup_heredoc_pipe(fd) == -1)
		return (-1);
	pid = protected_fork();
	if (pid == 0)
	{
		close(fd[0]);
		child_heredoc_process(fd[1], delimiter, shell);
	}
	close(fd[1]);
	return (wait_for_heredoc_child(pid, fd[0]));
}

/**
 * process_heredoc_input - Collects user input for a heredoc
 * @fd: File descriptor where collected input will be written
 * @delimiter: String that marks the end of heredoc input
 * @shell: Shell state information
 *
 * Purpose:
 * Reads lines of input from the user until the delimiter is encountered,
 * writing each line to the provided file descriptor.
 *
 * Technical details:
 * - Uses readline() to get input with proper line editing
 * - Detects EOF condition (user pressed Ctrl+D)
 * - Strips newline characters from input
 * - Compares each line to the delimiter
 * - Writes lines to the file descriptor
 * - Frees memory allocated by readline() to prevent leaks
 * - Resets the shell's heredoc flag when done
 *
 * Notes:
 * - This function handles the core input collection logic
 * - Each collected line is written with an added newline character
 * - The delimiter itself is not written to the output
 */
void	process_heredoc_input(int fd, char *delimiter, t_shell *shell)
{
	char	*line;
	char	*new_line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		new_line = ft_strchr(line, '\n');
		if (new_line)
			*new_line = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	shell->in_heredoc = 0;
}
