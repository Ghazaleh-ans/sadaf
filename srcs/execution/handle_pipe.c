/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:35:21 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 19:33:28 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * close_pipe_fds - Safely closes both pipe file descriptors
 */
static int	close_pipe_fds(int *fd)
{
	if (close_and_report(fd[0]) < 0 || close_and_report(fd[1]) < 0)
		return (-1);
	return (0);
}

/**
 * setup_pipe_input - Configures process to read from a pipe
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
 * regardless of whether the left command fails
 * 
 * Example: cat <missing | cat
 * - Left command (cat <missing) fails with exit code 1
 * - Right command (cat) succeeds with exit code 0  
 * - Pipe exit code should be 0 (from right command)
 */
static void	wait_for_children(int pid1, int pid2, t_shell *shell)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		shell->exit_status = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		shell->exit_status = 128 + WTERMSIG(status2);
}

/**
 * handle_pipe - Main function to execute piped commands
 * 
 * FIXED: Pipes should continue executing even if left command fails
 * The key insight is that both sides of the pipe should always run,
 * and only the right side's exit status matters for the overall exit code
 */
void	handle_pipe(t_pipecmd *pcmd, t_shell *shell)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (create_pipe(fd) < 0)
		exit(EXIT_FAILURE);
	setup_signals(1, shell);
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
	setup_signals(0, shell);
}
