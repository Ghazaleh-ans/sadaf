/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_leftcmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:00:55 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 17:27:12 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static void	child_left_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	if (setup_pipe_output(fd) < 0)
		exit(EXIT_FAILURE);
	runcmd(pcmd->left, shell);
	exit(EXIT_FAILURE);
}

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
