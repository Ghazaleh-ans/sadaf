/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:22:03 by gansari           #+#    #+#             */
/*   Updated: 2025/04/24 14:22:06 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static void	handle_builtin(t_execcmd *ecmd, t_shell *shell)
{
	int	exit_code;

	exit_code = exec_builtin(ecmd, shell);
	exit(exit_code);
}

static void	command_not_found(char *cmd)
{
	ft_putstr_fd("sadaf: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

static void	exec_external_command(char *path, char **argv, t_shell *shell)
{
	char	**env_array;

	env_array = env_to_array(shell->env_list);
	if (execve(path, argv, env_array) < 0)
	{
		free(path);
		cleanup_tokens(env_array);
		ft_perror("execve");
		exit(1);
	}
	free(path);
	cleanup_tokens(env_array);
}

void	execute_command(t_execcmd *ecmd, t_shell *shell)
{
	char	*path;

	if (!ecmd->argv[0])
		exit(0);
	expand_variables(ecmd, shell);
	if (is_builtin(ecmd->argv[0]))
		handle_builtin(ecmd, shell);
	path = find_command_path(ecmd->argv[0], shell);
	if (!path)
		command_not_found(ecmd->argv[0]);
	exec_external_command(path, ecmd->argv, shell);
}
