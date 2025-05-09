/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/09 07:26:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

void exec_command(char *buf, t_shell *shell)
{
	if (fork1() == 0)
	{
		t_cmd *cmd = parsecmd(buf);
		runcmd(cmd, shell);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}

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
