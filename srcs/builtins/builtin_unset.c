/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:37:15 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:37:20 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * Removes an environment variable from the linked list
 *
 * This function performs a linear search through the environment
 * linked list to find and remove the specified variable.
 *
 * Algorithm:
 * 1. Traverse the linked list to find the target variable
 * 2. Handle edge cases (removing from head vs middle/end)
 * 3. Properly link nodes to maintain list integrity
 * 4. Free allocated memory
 *
 * @param shell      Pointer to shell structure containing env_list
 * @param var_name   Name of the environment variable to remove
 *
 * Memory Management:
 * - Frees the node's name string
 * - Frees the node's value string
 * - Frees the node structure itself
 *
 * Edge Cases:
 * - Variable not found: Function returns without error
 * - Removing head node: Shell's env_list is updated
 * - Removing middle/end node: Previous node's next pointer is updated
 */
static void	remove_env_var(t_shell *shell, char *var_name)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->name, var_name) == 0)
		{
			if (!prev)
				shell->env_list = current->next;
			else
				prev->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

/**
 * Implements the 'unset' built-in command
 *
 * This function removes one or more environment variables from
 * the shell's environment. It supports multiple arguments,
 * processing each one sequentially.
 *
 * Usage: unset VAR1 [VAR2] [VAR3] ...
 *
 * Features:
 * - Accepts multiple variable names as arguments
 * - Silently ignores non-existent variables
 * - Always returns 0 (success) regardless of operation outcome
 *
 * @param ecmd  Execution command structure containing argv
 * @param shell Shell structure containing environment list
 * @return      Always returns 0 (success)
 *
 * Implementation Note:
 * This function follows POSIX behavior where unset always
 * succeeds, even if the variable doesn't exist.
 */
int	builtin_unset(t_execcmd *ecmd, t_shell *shell)
{
	int	i;

	i = 1;
	while (ecmd->argv[i])
	{
		remove_env_var(shell, ecmd->argv[i]);
		i++;
	}
	return (0);
}

/* ADDITIONAL CONTEXT COMMENTS:
 *
 * Data Structures:
 * - t_shell: Contains env_list (head of environment linked list)
 * - t_env: Node structure with name, value, and next pointer
 * - t_execcmd: Command structure with argv array
 *
 * Related Functions:
 * - ft_strcmp: Custom string comparison function
 * - free: Standard library function for memory deallocation
 *
 * Common Usage Examples:
 * unset PATH                    // Remove PATH variable
 * unset VAR1 VAR2 VAR3         // Remove multiple variables
 * unset NONEXISTENT            // Does nothing, no error
 *
 * Design Decisions:
 * 1. No error checking for arguments - POSIX compliance
 * 2. Linear search acceptable for small environment lists
 * 3. Silent failure for non-existent variables
 * 4. No output on successful removal
 */
