/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:16 by gansari           #+#    #+#             */
/*   Updated: 2025/05/15 15:57:50 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/*
 * open_file - Opens a file with specified access mode
 *
 * This function attempts to open a file with the given mode.
 * The function has special behavior based on the access mode:
 * - For read-only mode (O_RDONLY), it simply opens with the provided mode
 * - For other modes (write/append), it also sets file permissions to 0644,
 *   which represents "rw-r--r--" (readable by all, writable by owner)
 *
 * The function abstracts away the differences in open() calls between
 * read operations and write operations.
 *
 * Parameters:
 *   file - Path to the file to be opened
 *   mode - File access mode flags (e.g., O_RDONLY, O_WRONLY | O_CREAT)
 *
 * Return value:
 *   File descriptor (positive integer) on success, negative value on failure
 */
static int	open_file(char *file, int mode)
{
	int	fd;

	if (mode & O_RDONLY)
		fd = open(file, mode);
	else
		fd = open(file, mode, 0644);
	return (fd);
}

/*
 * print_error - Prints a formatted error message for file operations
 *
 * This function constructs and outputs a standardized error message when
 * file operations fail. It follows the shell convention of prefixing errors
 * with the shell name.
 *
 * The resulting format is: "sadaf: filename: error_message"
 * where error_message comes from the system error description (errno).
 *
 * The error is sent to stderr (file descriptor 2).
 *
 * Parameters:
 *   file - The filename that caused the error
 *
 * The function doesn't return a value. It uses custom string output
 * functions that write directly to file descriptors.
 */
static void	print_error(char *file)
{
	ft_putstr_fd("sadaf: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_perror("");
}

/*
 * handle_file_redirection - Implements file redirection for shell commands
 *
 * This function handles the actual redirection mechanism for standard 
 * input/output
 * to files. It's responsible for implementing the behavior of shell operators
 * like < (input redirection) and > (output redirection).
 *
 * Steps performed:
 * 1. Open the target file with appropriate mode (read/write/append)
 * 2. Check for opening errors and report them if they occur
 * 3. Replace the standard file descriptor (usually stdin/stdout) with the
 *    newly opened file using dup2()
 * 4. Handle any errors that occur during the dup2() operation
 * 5. Close the original file descriptor to avoid leaks
 *
 * Parameters:
 *   rcmd - Redirection command structure containing file info and mode
 *
 * The function exits the process with code 1 if any errors occur,
 * which is standard behavior for shell redirection failures.
 */
static void	handle_file_redirection(t_redircmd *rcmd)
{
	int	fd;

	fd = open_file(rcmd->file, rcmd->mode);
	if (fd < 0)
	{
		print_error(rcmd->file);
		exit(1);
	}
	if (dup2(fd, rcmd->fd) == -1)
	{
		ft_perror("sadaf: dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

/*
 * handle_fd_redirection - Redirects stdin to a provided file descriptor
 *
 * This function is used when we already have an open file descriptor that
 * needs to be connected to standard input. This is primarily used for
 * heredoc redirection, where a pipe has already been set up.
 *
 * Unlike handle_file_redirection, this function skips the file opening step
 * since it receives an already-open file descriptor.
 *
 * Steps performed:
 * 1. Redirect stdin (fd 0) to use the provided file descriptor
 * 2. Handle any errors during redirection
 * 3. Close the original file descriptor to prevent leaks
 *
 * Parameters:
 *   rcmd - Redirection command structure containing the file descriptor
 *
 * The function exits with code 1 if the dup2() operation fails,
 * following standard shell behavior for redirection errors.
 */
static void	handle_fd_redirection(t_redircmd *rcmd)
{
	if (dup2(rcmd->fd, STDIN_FILENO) == -1)
	{
		ft_perror("sadaf: dup2");
		exit(1);
	}
	close(rcmd->fd);
}

/*
 * handle_redirections - Main entry point for all redirection operations
 *
 * This function handles all types of redirections in the shell:
 * - Standard input redirection (<)
 * - Standard output redirection (> and >>)
 * - Here-document redirection (<<)
 *
 * The function determines the type of redirection needed based on the
 * command structure, and routes the execution to the appropriate handler.
 *
 * For heredocs, it sets up the input pipe. For file redirections, it
 * processes the file opening and redirection.
 *
 * After setting up all redirections, it executes the command with
 * the redirections in place.
 *
 * Parameters:
 *   rcmd - The redirection command structure
 *   shell - The shell state containing environment and other context
 *
 * This function doesn't return as it calls runcmd(), which
 * executes the command after redirection setup.
 */
void	handle_redirections(t_redircmd *rcmd, t_shell *shell)
{
	if (rcmd->file)
		handle_file_redirection(rcmd);
	else
		handle_fd_redirection(rcmd);
	runcmd(rcmd->cmd, shell);
}
