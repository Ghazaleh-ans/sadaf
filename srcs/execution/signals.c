/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:42:51 by gansari           #+#    #+#             */
/*   Updated: 2025/05/07 13:42:53 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	g_signal_received = 0;

/**
* Signal handler for SIGINT (Ctrl+C)
*
* Handles the interrupt signal sent by pressing Ctrl+C. Instead of terminating
* the shell, this handler provides a clean user experience by:
* 1. Recording the signal receipt in g_signal_received
* 2. Printing a newline for clean output formatting
* 3. Using readline to clear the current line and redisplay the prompt
*
* This allows users to cancel current input without exiting the shell.
*
* @param sig The signal number (ignored as we know it's SIGINT)
*/
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// void handle_sigquit(int sig)
// {
//     (void)sig;
//     // Do nothing for SIGQUIT in interactive mode
// }
/**
* Configure signal handling based on shell's operational mode
*
* Dynamically adjusts signal behavior to provide appropriate handling for
* different states of shell operation:
*
* Mode 0 (Interactive mode - waiting for user input):
*   - SIGINT (Ctrl+C): Custom handler to clear input without exiting
*   - SIGQUIT (Ctrl+\): Ignored to prevent accidental termination
*
* Mode 1 (Command execution):
*   - SIGINT and SIGQUIT: Default behavior to allow child process termination
*
* Mode 2 (Parent waiting for child):
*   - SIGINT and SIGQUIT: Ignored to prevent parent termination
*   - Ensures signals reach the intended child process
*
* @param mode The current operational mode (0, 1, or 2)
*/
void	setup_signals(int mode)
{
	if (mode == 0)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == 1)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (mode == 2)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
}
