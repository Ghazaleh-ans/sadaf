/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:22:03 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/15 17:35:31 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * print_prompt - Prints the shell prompt with the current working directory
 *
 * This function retrieves the current working directory and formats it
 * to display as part of the shell prompt. The prompt is printed to
 * standard error (stderr) for user input.
 *
 * The format of the prompt is:
 * "-> <current_directory> $ "
 *
 * If the current working directory cannot be retrieved, a default
 * prompt "-> ? $ " is displayed.
 */

void	print_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*es;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		es = cwd + ft_strlen(cwd);
		while (*es != '/')
			es--;
		es++;
		fprintf(stderr, "-> %s $ ", es);
	}
}

/**
 * getcmd - Reads a command from the user
 *
 * This function displays the shell prompt and waits for user input.
 * It uses the readline library to handle line editing and history.
 *
 * Returns: A pointer to the command string entered by the user,
 *          or NULL if an error occurs.
 */

char	*getcmd(void)
{
	char	cwd[PATH_MAX];
	char	prompt[PATH_MAX];
	char	*cmd;
	char	*es;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		es = ft_strrchr(cwd, '/');
		if (es)
			ft_snprintf(prompt, sizeof(prompt), "-> %s $ ", es + 1);
		else
			ft_snprintf(prompt, sizeof(prompt), "-> ? $ ");
	}
	else
		ft_snprintf(prompt, sizeof(prompt), "-> ? $ ");
	cmd = readline(prompt);
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}
