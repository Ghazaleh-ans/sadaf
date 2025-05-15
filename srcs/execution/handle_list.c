/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:22:41 by gansari           #+#    #+#             */
/*   Updated: 2025/05/15 15:52:16 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * handle_list - Executes command sequences separated by ';'
 *
 * This function handles sequential command execution in the shell.
 * When commands are separated by semicolons (e.g., "cmd1 ; cmd2"),
 * they should be executed one after another regardless of whether
 * previous commands succeeded or failed.
 *
 * The function follows this process:
 * 1. Forks a child process to execute the left command (cmd1)
 * 2. The parent process waits for the child to complete
 * 3. Captures the exit status from the left command
 * 4. Updates the shell's exit_status to reflect the left command's result
 * 5. Executes the right command (cmd2) in the current process
 *
 * This approach ensures proper isolation between commands - the left
 * command runs in its own process space, while the shell maintains
 * state (like exit status) between commands.
 *
 * Unlike pipes, there is no data sharing between the commands;
 * they are completely independent operations.
 *
 * For sequences of more than two commands (e.g., "cmd1 ; cmd2 ; cmd3"),
 * the parser creates a nested structure where each pair is treated
 * as a list command, and this function is called recursively.
 *
 * @param lcmd   Pointer to list command structure containing left 
 * and right commands
 * @param shell  Pointer to shell structure with environment and state 
 * information
 */
void	handle_list(t_listcmd *lcmd, t_shell *shell)
{
	int	pid;
	int	status;

	pid = protected_fork();
	if (pid == 0)
		runcmd(lcmd->left, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	pid = protected_fork();
	if (pid == 0)
		runcmd(lcmd->right, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}
