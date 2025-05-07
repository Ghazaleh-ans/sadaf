/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:39:03 by gansari           #+#    #+#             */
/*   Updated: 2025/05/06 14:47:39 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	setup_heredoc_pipe(int *fd)
{
	if (pipe(fd) == -1)
	{
		ft_perror("heredoc");
		return (-1);
	}
	return (0);
}

static void	child_heredoc_process(int fd_write, char *delimiter, t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	shell->in_heredoc = 1;
	process_heredoc_input(fd_write, delimiter, shell);
	close(fd_write);
	exit(0);
}

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
