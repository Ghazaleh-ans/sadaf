/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:01:09 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/27 16:07:40 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	handle_special_cases(char *cmd_no_quotes,
		t_execcmd *ecmd, t_shell *shell)
{
	if (is_complex_command(cmd_no_quotes))
	{
		try_execute_as_command(cmd_no_quotes, shell);
		return (1);
	}
	if (is_builtin(cmd_no_quotes))
	{
		handle_builtin(ecmd, shell);
		return (1);
	}
	if (check_if_directory(cmd_no_quotes)
		&& (ft_strncmp(cmd_no_quotes, "./", 2) == 0
			|| ft_strncmp(cmd_no_quotes, "../", 3) == 0))
	{
		handle_directory_error(cmd_no_quotes);
		return (1);
	}
	if (check_if_directory(cmd_no_quotes))
	{
		command_not_found(cmd_no_quotes);
		return (1);
	}
	return (0);
}

static void	execute_external_command_with_path(char *cmd_no_quotes,
		t_execcmd *ecmd, t_shell *shell)
{
	char	*path;

	path = find_command_path(cmd_no_quotes, shell);
	if (!path)
	{
		command_not_found(ecmd->argv[0]);
		return ;
	}
	exec_external_command(path, ecmd->argv, shell);
}

void	handle_command_execution(char *cmd_no_quotes, t_execcmd *ecmd,
		t_shell *shell)
{
	if (handle_special_cases(cmd_no_quotes, ecmd, shell))
		return ;
	execute_external_command_with_path(cmd_no_quotes, ecmd, shell);
}
