/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:22:03 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/06 18:50:00 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	print_prompt()
{
	char cwd[PATH_MAX];
	char	*es;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		es = cwd + strlen(cwd);
		while (*es != '/')
			es--;
		es++;
		fprintf(stderr, "-> %s $ ", es);
	}
}

char *getcmd(void)
{
	char cwd[PATH_MAX];
	char prompt[PATH_MAX];
	char *cmd;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		char *es = strrchr(cwd, '/');
		if (es)
			snprintf(prompt, sizeof(prompt), "-> %s $ ", es + 1);
		else
			snprintf(prompt, sizeof(prompt), "-> ? $ ");
	}
	else
		snprintf(prompt, sizeof(prompt), "-> ? $ ");

	cmd = readline(prompt);

	if (cmd && *cmd)
		add_history(cmd);

	return (cmd);  // caller must free(cmd)
}