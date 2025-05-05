/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:01 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:35:05 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * Implements the echo built-in command for the shell.
 *
 * The echo command outputs its arguments to standard output. It supports:
 * - The -n flag to suppress the trailing newline
 * - Multiple arguments separated by spaces
 * - Proper space handling between arguments
 *
 * @param ecmd: Structure containing command arguments
 * @param shell: Shell instance (unused in this function)
 * @return: Always returns 0 (success)
 */
int	builtin_echo(t_execcmd *ecmd, t_shell *shell)
{
	int	i;
	int	n_flag;

	(void)shell;
	n_flag = 0;
	i = 1;
	if (ecmd->argv[1] && ft_strcmp(ecmd->argv[1], "-n") == 0)
	{
		n_flag = 1;
		i = 2;
	}
	while (ecmd->argv[i])
	{
		ft_putstr_fd(ecmd->argv[i], STDOUT_FILENO);
		if (ecmd->argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
