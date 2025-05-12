/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:35:27 by gansari           #+#    #+#             */
/*   Updated: 2025/05/12 15:40:33 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int is_builtin_no_fork(const char *cmd)
{
	return (strcmp(cmd, "cd") == 0 ||
		strcmp(cmd, "exit") == 0 ||
		strcmp(cmd, "export") == 0 ||
		strcmp(cmd, "unset") == 0);
}

static int	should_fork(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	if (cmd->type != EXEC)
		return (1);
	ecmd = (t_execcmd *)cmd;
	return (!is_builtin_no_fork(ecmd->argv[0]));
}

static void execute_builtin(t_cmd *cmd, t_shell *shell)
{
	t_execcmd *ecmd = (t_execcmd *)cmd;
	int status = exec_builtin(ecmd, shell);
	shell->exit_status = status;
}

static void execute_forked(t_cmd *cmd, t_shell *shell)
{
	int status;

	if (protected_fork() == 0)
	{
		runcmd(cmd, shell);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	wait(&status);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}

void	execution(char *buf, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parsecmd(buf);
	if (collect_all_heredocs(cmd, shell) < 0)
	{
		free_cmd(cmd);
		return;
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
