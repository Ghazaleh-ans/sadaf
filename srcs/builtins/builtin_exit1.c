/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:55:50 by mukibrok          #+#    #+#             */
/*   Updated: 2025/06/03 18:59:34 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * cleanup_before_exit - Performs comprehensive cleanup before shell exit
 * @shell: Shell structure to clean up
 * 
 * This function ensures all allocated memory is freed before exit:
 * - Clears readline history
 * - Frees environment list
 * - Frees shell structure
 */

void	cleanup_before_exit(t_shell *shell)
{
	if (shell)
	{
		rl_clear_history();
		free_shell(shell);
	}
	else
		rl_clear_history();
}
