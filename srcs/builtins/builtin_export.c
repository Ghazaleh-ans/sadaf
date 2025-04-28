/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:32:47 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:32:50 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static void	print_env_vars(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(current->name, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(current->value, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
		current = current->next;
	}
}

static void	parse_export_arg(char *arg, char **name, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		*name = ft_strdup(arg);
		*value = ft_strdup("");
	}
	else
	{
		*name = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
}

static int	update_or_add_env(t_shell *shell, char *name, char *value)
{
	t_env	*current;
	int		found;
	t_env	*new_node;

	found = 0;
	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			found = 1;
			break ;
		}
		current = current->next;
	}
	if (!found)
	{
		new_node = create_env_node(name, value);
		if (new_node)
			add_env_node(shell, new_node);
	}
	return (0);
}

int	builtin_export(t_execcmd *ecmd, t_shell *shell)
{
	int		i;
	char	*name;
	char	*value;

	if (!ecmd->argv[1])
	{
		print_env_vars(shell->env_list);
		return (0);
	}
	i = 1;
	while (ecmd->argv[i])
	{
		parse_export_arg(ecmd->argv[i], &name, &value);
		update_or_add_env(shell, name, value);
		free(name);
		free(value);
		i++;
	}
	return (0);
}
