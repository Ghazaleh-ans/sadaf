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
