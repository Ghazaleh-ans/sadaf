/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:52:23 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/13 18:52:36 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	add_env_node(t_shell *shell, t_env *new_node)
{
	t_env	*current;

	if (!shell || !new_node)
		return ;
	if (!shell->env_list)
	{
		shell->env_list = new_node;
		return ;
	}
	current = shell->env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
