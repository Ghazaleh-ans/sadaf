/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:16 by gansari           #+#    #+#             */
/*   Updated: 2025/04/24 14:23:18 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	open_file(char *file, int mode)
{
	int	fd;

	if (mode & O_RDONLY)
		fd = open(file, mode);
	else
		fd = open(file, mode, 0644);
	return (fd);
}

static void	print_error(char *file)
{
	ft_putstr_fd("sadaf: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_perror("");
}

static void	handle_file_redirection(t_redircmd *rcmd)
{
	int	fd;

	close(rcmd->fd);
	fd = open_file(rcmd->file, rcmd->mode);
	if (fd < 0)
	{
		print_error(rcmd->file);
		exit(1);
	}
	if (dup2(fd, rcmd->fd) == -1)
	{
		ft_perror("sadaf: dup2");
		exit(1);
	}
	close(fd);
}

static void	handle_fd_redirection(t_redircmd *rcmd)
{
	if (dup2(rcmd->fd, STDIN_FILENO) == -1)
	{
		ft_perror("sadaf: dup2");
		exit(1);
	}
	close(rcmd->fd);
}

void	handle_redirections(t_redircmd *rcmd, t_shell *shell)
{
	if (rcmd->file)
		handle_file_redirection(rcmd);
	else
		handle_fd_redirection(rcmd);
	runcmd(rcmd->cmd, shell);
}
