/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_leftcmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:00:55 by gansari           #+#    #+#             */
/*   Updated: 2025/05/15 15:52:50 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * child_left_cmd - Handles execution in the left command's child process
 *
 * This function contains the logic executed by the child process for the
 * left-side command in a pipe (cmd1 in "cmd1 | cmd2"). It performs two 
 * main tasks:
 * 1. Sets up the pipe output redirection using setup_pipe_output()
 *    to ensure command output is directed to the pipe
 * 2. Executes the actual command using the runcmd() function
 *
 * If the redirection setup fails, the function exits with a failure status.
 * If runcmd() fails (which should not happen in normal operation as runcmd()
 * typically executes a binary and doesn't return), the function reports
 * the error and exits with failure status. This ensures that pipe execution
 * failures are properly propagated to the parent process.
 *
 * @param pcmd   Pointer to pipe command structure containing left and right 
 * commands
 * @param shell  Pointer to shell structure with environment variables and state
 * @param fd     Array containing pipe file descriptors [read_end, write_end]
 */
static void	child_left_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	if (setup_pipe_output(fd) < 0)
		exit(EXIT_FAILURE);
	runcmd(pcmd->left, shell);
	ft_error("runcmd failed in left pipe command");
	exit(EXIT_FAILURE);
}

/**
 * execute_left_cmd - Creates and configures child process for the left command
 *
 * This function creates a child process for the left side of a pipe expression
 * (the command before the pipe symbol). The function performs these operations:
 * 1. Calls fork() using the error-protected fork1() wrapper function
 * 2. In the child process (pid=0):
 *    - Delegates to child_left_cmd() to set up redirection and 
 * execute the command
 * 3. In the parent process, handles error conditions and returns the child PID
 *
 * Error handling is important for this function since fork() can fail in
 * resource-constrained environments. If fork fails, the function properly
 * reports the error and returns -1, allowing the caller to perform cleanup.
 * The successful return value (child PID) is used later for process tracking
 * and waiting.
 *
 * @param pcmd   Pointer to pipe command structure containing left and 
 * right commands
 * @param shell  Pointer to shell structure with environment variables and state
 * @param fd     Array containing pipe file descriptors [read_end, write_end]
 * @return       PID of the created child process, or -1 on error
 */
int	execute_left_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	int	pid;

	pid = protected_fork();
	if (pid == 0)
		child_left_cmd(pcmd, shell, fd);
	else if (pid < 0)
	{
		ft_perror("fork");
		return (-1);
	}
	return (pid);
}
