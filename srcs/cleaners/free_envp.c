/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:21:11 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/22 17:56:05 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * free_env_list - Cleans up environment variable storage
 * @env_list: Starting point of environment list
 *
 * Removes all environment entries:
 * - Frees variable names (like "PATH")
 * - Frees variable values (like "/usr/bin")
 * - Removes list structure itself
 */

void	free_env_list(t_env *env_list)
{
	t_env *temp;

	while (env_list)
	{
		temp = env_list;
		env_list = env_list->next;
		free(temp->name);
		free(temp->value);
		free(temp);
	}
}

/**
 * free_shell - Clears shell program memory
 * @shell: Main shell structure to clean
 *
 * Handles full cleanup:
 * - First clears environment list
 * - Then removes shell container
 * - Safe to call even if shell is NULL
 */

void	free_shell(t_shell *shell)
{
	if (shell)
	{
		free_env_list(shell->env_list);
		free(shell);
	}
}