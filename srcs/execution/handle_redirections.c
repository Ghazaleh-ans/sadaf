/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:16 by gansari           #+#    #+#             */
/*   Updated: 2025/05/27 17:44:29 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/*
 * open_file - Opens a file with specified access mode and proper error handling
 */
int	open_file(char *file, int mode)
{
	int	fd;

	if (mode & O_RDONLY)
		fd = open(file, mode);
	else
		fd = open(file, mode, 0644);
	return (fd);
}

/*
 * handle_fd_redirection - Redirects using an already-open file descriptor
 * (Used primarily for heredocs)
 */
static int	handle_fd_redirection(t_redircmd *rcmd)
{
	if (rcmd->fd < 0)
	{
		ft_putstr_fd("Error: Invalid heredoc file descriptor\n", STDERR_FILENO);
		return (-1);
	}
	if (dup2(rcmd->fd, STDIN_FILENO) == -1)
	{
		ft_perror("sadaf: dup2");
		close(rcmd->fd);
		return (-1);
	}
	close(rcmd->fd);
	return (0);
}

void	handle_redirections(t_redircmd *rcmd, t_shell *shell)
{
	int		result;
	t_cmd	*actual_cmd;

	if (validate_all_redirections(rcmd) < 0)
		exit(1);
	if (rcmd->file)
		result = handle_file_redirection(rcmd);
	else
		result = handle_fd_redirection(rcmd);
	if (result < 0)
		exit(1);
	actual_cmd = (t_cmd *)rcmd;
	while (actual_cmd && actual_cmd->type == REDIR)
		actual_cmd = ((t_redircmd *)actual_cmd)->cmd;
	runcmd(actual_cmd, shell);
}
