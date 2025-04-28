/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:26:17 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:26:21 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	exec_builtin(t_execcmd *ecmd, t_shell *shell)
{
	char	*cmd;

	cmd = ecmd->argv[0];
	if (!cmd)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (builtin_echo(ecmd, shell));
	if (ft_strcmp(cmd, "cd") == 0)
		return (builtin_cd(ecmd, shell));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (builtin_pwd(ecmd, shell));
	if (ft_strcmp(cmd, "export") == 0)
		return (builtin_export(ecmd, shell));
	if (ft_strcmp(cmd, "unset") == 0)
		return (builtin_unset(ecmd, shell));
	if (ft_strcmp(cmd, "env") == 0)
		return (builtin_env(ecmd, shell));
	if (ft_strcmp(cmd, "exit") == 0)
		return (builtin_exit(ecmd, shell));
	return (1);
}
