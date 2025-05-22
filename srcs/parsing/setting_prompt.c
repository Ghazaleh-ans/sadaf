/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:22:03 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/22 21:28:22 by gansari          ###   ########.fr       */
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

/**
 * getcmd - Reads a command from the user
 *
 * This function displays the shell prompt and waits for user input.
 * It uses the readline library to handle line editing and history.
 *
 * Returns: A pointer to the command string entered by the user,
 *          or NULL if an error occurs.
 */

void	build_prompt(char *prompt, size_t size)
{
	char	cwd[PATH_MAX];
	char	*es;

	if (getcwd(cwd, sizeof(cwd)))
	{
		es = ft_strrchr(cwd, '/');
		ft_snprintf(prompt, size,
			GREEN "Sadaf" RESET " " MAGENTA "🐚" RESET " -> "
			CYAN "%s" RESET " " YELLOW "$" RESET " ", es + 1);
	}
	else
		ft_snprintf(prompt, size,
			GREEN "Sadaf" RESET " " MAGENTA "-> ?" RESET " "
			YELLOW "$" RESET " ");
}

char	*getcmd(void)
{
	char	prompt[PATH_MAX * 2];
	char	*cmd;

	build_prompt(prompt, sizeof(prompt));
	cmd = readline(prompt);
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}
