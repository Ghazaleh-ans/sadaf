/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:35:21 by gansari           #+#    #+#             */
/*   Updated: 2025/05/07 14:11:18 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * create_pipe - Creates a pipe and handles errors
 *
 * This function wraps the pipe() system call with proper error handling.
 * It attempts to create a new pipe, which consists of two file descriptors:
 * fd[0] for reading from the pipe and fd[1] for writing to the pipe.
 * If the pipe creation succeeds, these descriptors are stored in the
 * provided array.
 *
 * Pipes are fundamental to the shell's ability to connect commands together.
 * They create a unidirectional communication channel where data written to
 * fd[1] can be read from fd[0]. This allows output from one process to become
 * input for another process, which is the core mechanism behind shell pipelines.
 *
 * If pipe creation fails (which might happen in resource-constrained environments),
 * the function reports the error through ft_perror and returns -1, allowing
 * the caller to handle the error condition appropriately.
 *
 * @param fd  Array to store pipe file descriptors [read_end, write_end]
 * @return    0 on success, -1 on error
 */
int		create_pipe(int *fd)
{
	if (pipe(fd) < 0)
	{
		ft_perror("pipe");
		return (-1);
	}
	return (0);
}

/**
 * cleanup_pipe - Closes pipe file descriptors and kills processes on error
 *
 * This function provides comprehensive cleanup for pipe resources in error
 * conditions. It performs two main tasks:
 * 1. Closes both pipe file descriptors using safe_close() to prevent leaks
 * 2. If a valid PID is provided (pid1 > 0), it terminates that process and
 *    waits for it to exit to avoid creating zombie processes
 *
 * The function is specifically designed for error recovery scenarios in the
 * pipe handling process. For example, if creating the right command process fails,
 * this function is called to clean up the pipe resources and terminate the already
 * running left command process. This ensures that all resources are properly
 * released and no orphaned processes remain in the system, even when errors occur.
 *
 * @param fd    Array containing pipe file descriptors [read_end, write_end]
 * @param pid1  PID of the left command process to kill on error, or -1 if not applicable
 */
void	cleanup_pipe(int *fd, int pid1)
{
	safe_close(fd[0]);
	safe_close(fd[1]);
	if (pid1 > 0)
	{
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
	}
}

/**
 * safe_close - Safely closes a file descriptor and handles errors
 *
 * This utility function closes a file descriptor and logs any errors.
 * Unlike close_and_report(), this function doesn't return an error code
 * but instead just logs errors if they occur. This is particularly useful
 * in cleanup scenarios where we want to attempt to close file descriptors
 * but don't need to take additional action if the close fails.
 *
 * This function is primarily used in the parent process after forking
 * the child processes, as well as during cleanup operations. It helps
 * avoid code duplication while ensuring all file descriptor handling
 * follows consistent patterns throughout the codebase.
 *
 * @param fd  File descriptor to close
 */
void	safe_close(int fd)
{
	if (close(fd) < 0)
		ft_perror("close");
}

/**
 * close_and_report - Closes a file descriptor and reports errors
 *
 * This function wraps the close() system call with error handling.
 * When a file descriptor needs to be closed, this function attempts the operation
 * and properly reports any errors using the ft_perror utility function. It is used
 * throughout the pipe handling code to ensure consistent error handling and to
 * avoid code duplication.
 *
 * @param fd  File descriptor to close
 * @return    0 on success, -1 on error (sets errno)
 */
int		close_and_report(int fd)
{
	if (close(fd) < 0)
	{
		ft_perror("close");
		return (-1);
	}
	return (0);
}

/**
 * dup_and_report - Duplicates a file descriptor and reports errors
 *
 * This function wraps the dup() system call with error handling.
 * It duplicates the specified file descriptor, making a copy that refers
 * to the same open file description. This is a critical operation in pipe
 * redirection, as it allows the shell to replace standard input/output with
 * pipe endpoints. The function reports any errors that occur during duplication
 * using the ft_perror utility.
 *
 * @param fd  File descriptor to duplicate
 * @return    0 on success, -1 on error (sets errno)
 */
int		dup_and_report(int fd)
{
	if (dup(fd) < 0)
	{
		ft_perror("dup");
		return (-1);
	}
	return (0);
}
