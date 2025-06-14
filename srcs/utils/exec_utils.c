/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:35:27 by gansari           #+#    #+#             */
/*   Updated: 2025/06/06 17:27:18 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	is_builtin_no_fork(const char *cmd)
{
	return (ft_strcmp((char *)cmd, "cd") == 0
		|| ft_strcmp((char *)cmd, "exit") == 0
		|| ft_strcmp((char *)cmd, "export") == 0
		|| ft_strcmp((char *)cmd, "unset") == 0);
}

static int	should_fork(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	if (cmd->type != EXEC)
		return (1);
	ecmd = (t_execcmd *)cmd;
	return (!is_builtin_no_fork(ecmd->argv[0]));
}

static void	execute_forked(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	pid;

	pid = protected_fork();
	if (pid == 0)
	{
		setup_signals(1, shell);
		runcmd(cmd, shell);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	setup_signals(2, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
	setup_signals(0, shell);
}

/* Updated exec_utils.c - execution function */

void	execution(char *buf, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parsecmd(buf);
	if (!cmd)
	{
		shell->exit_status = 2;
		return;
	}
	
	// FIXED: Collect heredocs with proper signal handling
	if (collect_all_heredocs(cmd, shell) < 0)
	{
		free_cmd(cmd);
		
		// Check if it was interrupted by a signal
		if (g_signal_received)
		{
			shell->exit_status = 130;  // Standard exit code for Ctrl+C
			g_signal_received = 0;     // Reset the signal flag
			
			// Make sure we're back to normal signal handling
			setup_signals(0, shell);
		}
		return;
	}
	
	if (!should_fork(cmd))
	{
		if (cmd->type == EXEC)
			execute_builtin(cmd, shell);
	}
	else
		execute_forked(cmd, shell);
	free_cmd(cmd);
}
