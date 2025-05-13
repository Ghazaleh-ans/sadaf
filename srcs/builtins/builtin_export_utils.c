/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:30:52 by gansari           #+#    #+#             */
/*   Updated: 2025/05/13 15:31:31 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static t_env	*find_env_node(t_env *list, char *name)
{
	while (list)
	{
		if (ft_strcmp(list->name, name) == 0)
			return (list);
		list = list->next;
	}
	return (NULL);
}

static int	update_env_value(t_env *node, char *value)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return (-1);
	free(node->value);
	node->value = new_value;
	return (0);
}

static int	create_and_add_env(t_shell *shell, char *name, char *value)
{
	char	*new_name;
	char	*new_value;
	t_env	*new_node;

	new_name = ft_strdup(name);
	new_value = ft_strdup(value);
	if (!new_name || !new_value)
	{
		free(new_name);
		free(new_value);
		return (-1);
	}
	new_node = create_env_node(new_name, new_value);
	if (!new_node)
		return (-1);
	add_env_node(shell, new_node);
	return (0);
}

/**
 * update_or_add_env - Updates existing or adds new environment variable
 * @shell: Shell state containing environment list
 * @name: Variable name to set/update
 * @value: Value to assign to the variable
 *
 * This function:
 * 1. Searches for existing variable with same name
 * 2. If found, updates its value
 * 3. If not found, creates and adds new node
 */
int	update_or_add_env(t_shell *shell, char *name, char *value)
{
	t_env	*existing;

	existing = find_env_node(shell->env_list, name);
	if (existing)
		return (update_env_value(existing, value));
	return (create_and_add_env(shell, name, value));
}
