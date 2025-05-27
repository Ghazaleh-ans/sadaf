/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:28:53 by gansari           #+#    #+#             */
/*   Updated: 2025/05/20 14:28:59 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

char	**allocate_unquoted_array(char **argv, char *path)
{
	int		i;
	char	**unquoted_argv;

	i = 0;
	while (argv[i])
		i++;
	unquoted_argv = malloc(sizeof(char *) * (i + 1));
	if (!unquoted_argv)
	{
		free(path);
		ft_perror("malloc");
		exit(1);
	}
	return (unquoted_argv);
}

void	handle_unquote_error(char **unquoted_argv, char *path)
{
	cleanup_tokens(unquoted_argv);
	free(path);
	ft_perror("remove_quotes");
	exit(1);
}

void	check_cmd_args(t_execcmd *ecmd, t_shell *shell)
{
	if (!ecmd || !shell)
	{
		ft_error("execute_command: NULL pointer");
		exit(1);
	}
	if (!ecmd->argv[0])
		exit(0);
}

void	command_not_found(char *cmd)
{
	if (!cmd)
		cmd = "(null)";
	ft_putstr_fd("\x1b[31msadaf: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void	handle_directory_error(char *cmd)
{
	if (!cmd)
		cmd = "(null)";
	ft_putstr_fd("\x1b[31msadaf: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	exit(126);
}
