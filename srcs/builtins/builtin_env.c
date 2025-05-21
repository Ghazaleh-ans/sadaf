/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:32:16 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:32:23 by gansari          ###   ########.fr       */
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
		return(126);
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
/*
 * DATA STRUCTURES USED:
 *
 * t_execcmd: Structure containing command execution data
 *   - type: Command type (EXEC in this case)
 *   - argv: Array of argument strings
 *   - eargv: Array of argument end pointers (used in parsing)
 *
 * t_shell: Structure containing shell state
 *   - env_list: Linked list of environment variables
 *   - exit_status: Last command exit status
 *   - in_heredoc: Flag indicating if in here document mode
 *
 * t_env: Structure for environment variable nodes
 *   - name: Environment variable name
 *   - value: Environment variable value
 *   - next: Pointer to next environment variable in list
 */

/*
 * EXAMPLE USAGE:
 *
 * Shell Input: "env"
 * Output:
 *   HOME=/home/user
 *   PATH=/usr/bin:/bin
 *   SHELL=/bin/sadaf
 *   ...
 *
 * Shell Input: "env extra args"
 * Output: (same as above, arguments are ignored)
 *   HOME=/home/user
 *   PATH=/usr/bin:/bin
 *   ...
 */

/*
 * ERROR HANDLING:
 *
 * This implementation has robust error handling:
 * - If shell->env_list is NULL, function safely handles empty list
 * - ft_putstr_fd handles NULL pointers safely
 * - No dynamic memory allocation, so no memory leaks
 * - Returns 0 always, as 'env' rarely fails
 */
