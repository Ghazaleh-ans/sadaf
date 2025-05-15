/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_rightcmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:03:24 by gansari           #+#    #+#             */
/*   Updated: 2025/05/15 16:05:07 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * child_right_cmd - Handles execution in the right command's child process
 *
 * This function contains the logic executed by the child process for the
 * right-side command in a pipe (cmd2 in "cmd1 | cmd2"). It performs two main
 *  tasks:
 * 1. Sets up the pipe input redirection using setup_pipe_input()
 *    to ensure command input comes from the pipe
 * 2. Executes the actual command using the runcmd() function
 *
 * The function sets up the right command to read its input from the read end
 * of the pipe, effectively capturing the output of the left command as its 
 * input.
 * If redirection setup fails, the function exits with a failure status.
 * If runcmd() fails (which should not happen in normal operation as runcmd()
 * typically executes a binary and doesn't return), the function reports
 * the error and exits with failure status. This ensures that pipe execution
 * failures are properly propagated to the parent process.
 *
 * @param pcmd   Pointer to pipe command structure containing left and right 
 * commands
 * @param shell  Pointer to shell structure with environment variables and 
 * state
 * @param fd     Array containing pipe file descriptors [read_end, write_end]
 */
static void	child_right_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	if (setup_pipe_input(fd) < 0)
		exit(EXIT_FAILURE);
	runcmd(pcmd->right, shell);
	ft_error("runcmd failed in right pipe command");
	exit(EXIT_FAILURE);
}

/**
 * execute_right_cmd - Creates and configures child process for the right 
 * command
 *
 * This function creates a child process for the right side of a pipe expression
 * (the command after the pipe symbol). The function performs these operations:
 * 1. Calls fork() using the error-protected fork1() wrapper function
 * 2. In the child process (pid=0):
 *    - Delegates to child_right_cmd() to set up redirection and execute
 *  the command
 * 3. In the parent process, handles error conditions and returns the child PID
 *
 * The right command process is critical because its exit status becomes the
 * exit status of the entire pipe command chain. The function properly handles
 * fork() failures by reporting errors and returning -1, allowing the caller
 * to perform cleanup including terminating the already-running left command
 * process to prevent orphaned processes. The successful return value (PID)
 * is used later for process tracking and waiting.
 *
 * @param pcmd   Pointer to pipe command structure containing left and right 
 * commands
 * @param shell  Pointer to shell structure with environment variables and 
 * state
 * @param fd     Array containing pipe file descriptors [read_end, write_end]
 * @return       PID of the created child process, or -1 on error
 */

int	execute_right_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	int	pid;

	pid = protected_fork();
	if (pid == 0)
		child_right_cmd(pcmd, shell, fd);
	else if (pid < 0)
	{
		ft_perror("fork");
		return (-1);
	}
	return (pid);
}
