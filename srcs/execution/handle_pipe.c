/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:35:21 by gansari           #+#    #+#             */
/*   Updated: 2025/05/06 14:48:35 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static void	setup_pipe_input(int *fd)
{
	close(0);
	dup(fd[0]);
	close(fd[0]);
	close(fd[1]);
}

static void	setup_pipe_output(int *fd)
{
	close(1);
	dup(fd[1]);
	close(fd[0]);
	close(fd[1]);
}

static int	execute_left_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	int	pid;

	pid = protected_fork();
	if (pid == 0)
	{
		setup_pipe_output(fd);
		runcmd(pcmd->left, shell);
	}
	return (pid);
}

static int	execute_right_cmd(t_pipecmd *pcmd, t_shell *shell, int *fd)
{
	int	pid;

	pid = protected_fork();
	if (pid == 0)
	{
		setup_pipe_input(fd);
		runcmd(pcmd->right, shell);
	}
	return (pid);
}

void	handle_pipe(t_pipecmd *pcmd, t_shell *shell)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (pipe(fd) < 0)
		ft_error("pipe");
	pid1 = execute_left_cmd(pcmd, shell, fd);
	pid2 = execute_right_cmd(pcmd, shell, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	wait(NULL);
}
