/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/13 16:08:33 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

// void execution(char *buf, t_shell *shell)
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
	if (!buf)
		return 0;

	buf[strcspn(buf, "\n")] = 0;

	if (strncmp(buf, "cd ", 3) == 0 || strcmp(buf, "cd") == 0)
	{
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
		if (buf[0] == '\n' || buf[0] == '\0')
		{
			free(buf);
			continue;
		}

		if (handle_cd(buf))
		{
			free(buf);
			continue;
		}
		execution(buf, shell);
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
