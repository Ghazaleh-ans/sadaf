/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:35:27 by gansari           #+#    #+#             */
/*   Updated: 2025/05/16 12:06:27 by gansari          ###   ########.fr       */
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

static void	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	t_execcmd	*ecmd;
	int			status;

	ecmd = (t_execcmd *)cmd;
	status = exec_builtin(ecmd, shell);
	shell->exit_status = status;
}

static void	execute_forked(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	pid;

	pid = protected_fork();
	if (pid == 0)
	{
		setup_signals(1);
		runcmd(cmd, shell);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	setup_signals(2);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
	setup_signals(0);
	g_signal_received = 0;
}

void	execution(char *buf, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parsecmd(buf);
	if (collect_all_heredocs(cmd, shell) < 0)
	{
		free_cmd(cmd);
		return ;
	}
	if (!should_fork(cmd))
	{
		if (cmd->type == EXEC)
			execute_builtin(cmd, shell);
	}
	else
	{
		execute_forked(cmd, shell);
	}
	free_cmd(cmd);
}
