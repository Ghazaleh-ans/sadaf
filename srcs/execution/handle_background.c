/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:34:44 by gansari           #+#    #+#             */
/*   Updated: 2025/05/07 15:05:54 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * handle_background - Executes a command in the background
 *
 * This function implements background command execution (commands ending with '&').
 * It creates a child process using protected_fork() to execute the command.
 * The parent process continues immediately without waiting for the child,
 * allowing the background command to run independently.
 *
 * Background execution works by:
 * 1. Creating a new process using protected_fork() (which handles fork errors)
 * 2. In the child process (pid == 0), executing the command via runcmd()
 * 3. The parent process immediately returns without calling wait(),
 *    which allows it to continue processing other commands
 *
 * @param bcmd   Pointer to the background command structure
 * @param shell  Pointer to the shell structure with environment and state
 */
void handle_background(t_backcmd *bcmd, t_shell *shell)
{
	int	pid;

	pid = protected_fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		runcmd(bcmd->cmd, shell);
	}
	exit(EXIT_FAILURE);
}
