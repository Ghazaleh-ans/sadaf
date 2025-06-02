/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:32:16 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 12:18:39 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * builtin_env - Prints all environment variables to standard output
 *
 * This function implements the 'env' command, which displays all currently
 * set environment variables in the format "NAME=VALUE".
 *
 * Implementation Details:
 * - Traverses the linked list of environment variables stored in shell->env_list
 * - Prints each environment variable in the standard format
 * - Uses file descriptor STDOUT_FILENO for output (typically 1)
 * - Ignores the command arguments (standard env behavior)
 *
 * @param ecmd    Pointer to the command structure containing arguments
 * @param shell   Pointer to the shell structure containing environment list
 * @return        Always returns 0 (success)
 *
 * UNIX Behavior:
 * - Without arguments: prints all environment variables
 * - With arguments: standard env can run commands with modified environment,
 *   but this implementation simply ignores arguments
 */
int	builtin_env(t_execcmd *ecmd, t_shell *shell)
{
	t_env	*current;
	int		i;

	i = 0;
	while (ecmd->argv[i])
		i++;
	if (i > 1)
	{
		fprintf(stderr, "env: %s: No such file or directory\n", ecmd->argv[1]);
		return (126);
	}
	current = shell->env_list;
	while (current)
	{
		ft_putstr_fd(current->name, STDOUT_FILENO);
		ft_putstr_fd("=", STDOUT_FILENO);
		ft_putstr_fd(current->value, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		current = current->next;
	}
	return (0);
}
