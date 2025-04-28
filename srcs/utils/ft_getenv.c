/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:11:41 by gansari           #+#    #+#             */
/*   Updated: 2025/04/24 15:11:43 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

char	*ft_getenv(char *name, t_shell *shell)
{
	t_env	*current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	add_env_node(t_shell *shell, t_env *new_node)
{
	t_env	*current;

	if (!shell || !new_node)
		return;
	if (!shell->env_list)
	{
		shell->env_list = new_node;
		return;
	}
	current = shell->env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
