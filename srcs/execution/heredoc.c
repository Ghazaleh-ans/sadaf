/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:39:03 by gansari           #+#    #+#             */
/*   Updated: 2025/06/06 17:25:37 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * setup_heredoc_pipe - Creates a pipe for heredoc data transfer
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
 * FIXED: Proper signal handling for heredoc child processes
 */
static void	child_heredoc_process(int fd_write, char *delimiter, t_shell *shell)
{
	// Set up signal handling for heredoc input
	signal(SIGINT, SIG_DFL);  // Allow Ctrl+C to terminate this process
	signal(SIGQUIT, SIG_IGN); // Ignore Ctrl+\ in heredoc
	
	shell->in_heredoc = 1;
	process_heredoc_input(fd_write, delimiter, shell);
	close(fd_write);
	exit(0);
}

/**
 * wait_for_heredoc_child - FIXED: Better signal handling during wait
 */
static int	wait_for_heredoc_child(int pid, int fd_read)
{
	int	status;
	int	result;

	// Wait for the heredoc child process
	result = waitpid(pid, &status, 0);
	
	if (result == -1)
	{
		close(fd_read);
		return (-1);
	}
	
	// Check if child was terminated by a signal (like Ctrl+C)
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			// Child was interrupted by Ctrl+C
			g_signal_received = 1;
			close(fd_read);
			return (-1);
		}
	}
	
	// Check if child exited with an error
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(fd_read);
		return (-1);
	}
	
	return (fd_read);
}

/**
 * handle_heredoc - FIXED: Better error handling and signal coordination
 */
int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int	fd[2];
	int	pid;

	// Reset signal flag before starting heredoc
	g_signal_received = 0;
	
	if (setup_heredoc_pipe(fd) == -1)
		return (-1);
		
	// Set up signals for parent during heredoc collection
	signal(SIGINT, SIG_IGN);  // Ignore Ctrl+C in parent during heredoc
	
	pid = protected_fork();
	if (pid == 0)
	{
		close(fd[0]);
		child_heredoc_process(fd[1], delimiter, shell);
	}
	
	close(fd[1]);
	
	// Wait for child and handle signals properly
	int result = wait_for_heredoc_child(pid, fd[0]);
	
	// Restore normal signal handling
	setup_signals(0, shell);
	
	return (result);
}

/**
 * process_heredoc_input - FIXED: Better signal handling during input
 */
void	process_heredoc_input(int fd, char *delimiter, t_shell *shell)
{
	char	*line;
	char	*new_line;

	while (1)
	{
		line = readline("> ");
		
		// Check for EOF (Ctrl+D) or signal interruption
		if (!line)
		{
			// EOF encountered
			break;
		}
		
		// Remove newline if present
		new_line = ft_strchr(line, '\n');
		if (new_line)
			*new_line = '\0';
			
		// Check if we've reached the delimiter
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		
		// Write the line to the pipe
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	
	shell->in_heredoc = 0;
}
