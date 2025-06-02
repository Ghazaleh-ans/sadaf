/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_shell_script.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:11:54 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 15:17:23 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static char	**create_script_argv(char *path, char **argv)
{
	char	**script_argv;
	int		i;

	script_argv = malloc(sizeof(char *) * (MAXARGS + 2));
	if (!script_argv)
		return (NULL);
	script_argv[0] = "/bin/sh";
	script_argv[1] = path;
	i = 1;
	while (argv[i] && i < MAXARGS - 1)
	{
		script_argv[i + 1] = argv[i];
		i++;
	}
	script_argv[i + 1] = NULL;
	return (script_argv);
}

static void	cleanup_and_exit(char **script_argv, char **env_array)
{
	cleanup_tokens(env_array);
	free(script_argv);
	exit(1);
}

void	try_execute_as_shell_script(char *path, char **argv, t_shell *shell)
{
	char	**script_argv;
	char	**env_array;

	script_argv = create_script_argv(path, argv);
	if (!script_argv)
		exit(1);
	env_array = env_to_array(shell->env_list);
	if (!env_array)
	{
		free(script_argv);
		exit(1);
	}
	execve("/bin/sh", script_argv, env_array);
	cleanup_and_exit(script_argv, env_array);
}
