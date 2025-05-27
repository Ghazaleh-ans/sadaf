/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:46:33 by gansari           #+#    #+#             */
/*   Updated: 2025/05/27 12:46:36 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	check_if_directory(char *path)
{
	struct stat	path_stat;

	if (!path || !*path)
		return (0);
	if (stat(path, &path_stat) == 0)
		return (S_ISDIR(path_stat.st_mode));
	return (0);
}

char	**prepare_unquoted_args(char **argv, char *path)
{
	char	**unquoted_argv;
	int		i;

	unquoted_argv = allocate_unquoted_array(argv, path);
	i = 0;
	while (argv[i])
	{
		unquoted_argv[i] = remove_quotes(argv[i]);
		if (!unquoted_argv[i])
			handle_unquote_error(unquoted_argv, path);
		i++;
	}
	unquoted_argv[i] = NULL;
	return (unquoted_argv);
}

void	exec_external_command(char *path, char **argv, t_shell *shell)
{
	char	**env_array;
	char	**unquoted_argv;

	unquoted_argv = prepare_unquoted_args(argv, path);
	env_array = env_to_array(shell->env_list);
	if (!env_array)
	{
		free(path);
		cleanup_tokens(unquoted_argv);
		ft_perror("env_to_array");
		exit(1);
	}
	if (execve(path, unquoted_argv, env_array) < 0)
	{
		free(path);
		cleanup_tokens(unquoted_argv);
		cleanup_tokens(env_array);
		ft_perror("execve");
		exit(1);
	}
}

void	setup_builtin_cmd(t_execcmd *cmd, char **tokens)
{
	int	i;

	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	i = 0;
	while (tokens[i] && i < MAXARGS - 1)
	{
		cmd->argv[i] = tokens[i];
		i++;
	}
	cmd->argv[i] = NULL;
}

void	handle_builtin_tokens(char **tokens, t_shell *shell)
{
	t_execcmd	cmd;
	int			status;

	setup_builtin_cmd(&cmd, tokens);
	status = exec_builtin(&cmd, shell);
	cleanup_tokens(tokens);
	exit(status);
}
