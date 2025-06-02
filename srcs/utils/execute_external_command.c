/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:28:12 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 15:38:23 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static void	cleanup_and_exit_env_error(char *path, char **unquoted_argv)
{
	free(path);
	cleanup_tokens(unquoted_argv);
	ft_perror("env_to_array");
	exit(1);
}

static void	handle_execve_eacces(char *path)
{
	ft_perror(path);
	free(path);
	exit(126);
}

static void	handle_execve_other_error(char *path)
{
	ft_perror(path);
	free(path);
	exit(127);
}

static void	handle_execve_error(int saved_errno, char *path, 
					char **argv, t_shell *shell)
{
	if (saved_errno == ENOEXEC)
		try_execute_as_shell_script(path, argv, shell);
	else if (saved_errno == EACCES)
		handle_execve_eacces(path);
	else
		handle_execve_other_error(path);
}

void	exec_external_command(char *path, char **argv, t_shell *shell)
{
	char	**env_array;
	char	**unquoted_argv;
	int		saved_errno;

	unquoted_argv = prepare_unquoted_args(argv, path);
	env_array = env_to_array(shell->env_list);
	if (!env_array)
		cleanup_and_exit_env_error(path, unquoted_argv);
	if (execve(path, unquoted_argv, env_array) < 0)
	{
		saved_errno = errno;
		cleanup_tokens(unquoted_argv);
		cleanup_tokens(env_array);
		handle_execve_error(saved_errno, path, argv, shell);
	}
}
