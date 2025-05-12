/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/12 15:18:08 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"
int	should_fork(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	if (cmd->type != EXEC)
		return (1);
	ecmd = (t_execcmd *)cmd;
	if (strcmp(ecmd->argv[0], "cd") == 0 ||
		strcmp(ecmd->argv[0], "exit") == 0 ||
		strcmp(ecmd->argv[0], "export") == 0 ||
		strcmp(ecmd->argv[0], "unset") == 0)
	{
		return (0);
	}
	return (1);
}

void	exec_command(char *buf, t_shell *shell)
{
	t_cmd	*cmd;
	int		status;

	cmd = parsecmd(buf);
	if (collect_all_heredocs(cmd, shell) < 0)
	{
		free_cmd(cmd);
		return;
	}
	if (!should_fork(cmd))
	{
		if (cmd->type == EXEC)
		{
			t_execcmd *ecmd = (t_execcmd *)cmd;
			status = exec_builtin(ecmd, shell);
			shell->exit_status = status;
		}
		free_cmd(cmd);
		return;
	}
	if (protected_fork() == 0)
	{
		runcmd(cmd, shell);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	wait(&status);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	free_cmd(cmd);
}

// void exec_command(char *buf, t_shell *shell)
// {
// 	t_cmd	*cmd;

// 	cmd = parsecmd(buf);
// 	if (collect_all_heredocs(cmd, shell) < 0)
// 	{
// 		free_cmd(cmd);
// 		return;
// 	}
// 	if (protected_fork() == 0)
// 	{
// 		runcmd(cmd, shell);
// 		free_cmd(cmd);
// 		exit(EXIT_SUCCESS);
// 	}
// 	wait(NULL);
// 	free_cmd(cmd);
// }

int handle_cd(char *buf)
{
	if (strncmp(buf, "cd ", 3) == 0 || strcmp(buf, "cd") == 0)
	{
		buf[strcspn(buf, "\n")] = 0;
		char *path = buf + 2;
		while (*path == ' ')
			path++;
		if (*path == 0)
		{
			path = getenv("HOME");
			if (!path) path = "/";
		}
		if (chdir(path) < 0)
			fprintf(stderr, "cannot cd to '%s'\n", path);
		return (1);
	}
	return (0);
}

void shell_loop(t_shell *shell)
{
	char *buf;

	while (1)
	{
		buf = getcmd();
		if (!buf)
			break;
		if (handle_cd(buf))
		{
			free(buf);
			continue;
		}
		exec_command(buf, shell);
		free(buf);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_shell *shell;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (perror("Failed to initialize shell"), EXIT_FAILURE);
	setup_signals(0);
	shell_loop(shell);
	free_shell(shell);
	return (EXIT_SUCCESS);
}
