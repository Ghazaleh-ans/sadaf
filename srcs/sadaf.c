/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/15 16:43:36 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

static int	is_only_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

int	handle_cd(char *buf)
{
	char	*path;

	if (!buf)
		return (0);
	buf[strcspn(buf, "\n")] = 0;
	if (strncmp(buf, "cd ", 3) == 0 || strcmp(buf, "cd") == 0)
	{
		path = buf + 2;
		while (*path == ' ')
			path++;
		if (*path == 0)
		{
			path = getenv("HOME");
			if (!path)
				path = "/";
		}
		if (chdir(path) < 0)
			fprintf(stderr, "cannot cd to '%s'\n", path);
		return (1);
	}
	return (0);
}

void	shell_loop(t_shell *shell)
{
	char	*buf;

	while (1)
	{
		buf = getcmd();
		if (!buf)
			break ;
		if (is_only_whitespace(buf))
		{
			free(buf);
			continue ;
		}
		if (buf[0] == '\n' || buf[0] == '\0')
		{
			free(buf);
			continue ;
		}
		if (handle_cd(buf))
		{
			free(buf);
			continue ;
		}
		execution(buf, shell);
		free(buf);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

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
