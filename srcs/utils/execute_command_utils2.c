
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:46:33 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 15:00:00 by gansari          ###   ########.fr       */
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
