/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:32:47 by gansari           #+#    #+#             */
/*   Updated: 2025/05/13 15:08:23 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * print_env_vars - Prints all environment variables with export format
 * @env_list: Linked list of environment variables
 *
 * This function displays all environment variables in the format:
 * declare -x NAME="VALUE"
 * This matches bash's behavior when 'export' is called without arguments
 */
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

/**
 * parse_export_arg - Parses an export argument into name and value
 * @arg: The export argument (e.g., "VAR=value" or "VAR")
 * @name: Output parameter for variable name
 * @value: Output parameter for variable value
 *
 * This function handles two cases:
 * 1. "VAR=value" - splits at '=' character
 * 2. "VAR" - creates an empty string value
 */
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
static int	update_or_add_env(t_shell *shell, char *name, char *value)
{
	t_env	*current;
	int		found;
	t_env	*new_node;
	char	*new_value;

	found = 0;
	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			new_value = ft_strdup(value);
			if (!new_value)
				return (-1);
			free(current->value);
			current->value = new_value;
			found = 1;
			break;
		}
		current = current->next;
	}
	if (!found)
	{
		char *new_name = ft_strdup(name);
		new_value = ft_strdup(value);
		if (!new_name || !new_value)
		{
			free(new_name);
			free(new_value);
			return (-1);
		}
		new_node = create_env_node(new_name, new_value);
		if (new_node)
			add_env_node(shell, new_node);
		else
			return (-1);
	}
	return (0);
}

/**
 * builtin_export - Implements the export builtin command
 * @ecmd: Command structure containing arguments
 * @shell: Shell state
 *
 * Behavior:
 * - No arguments: display all environment variables
 * - With arguments: set/update environment variables
 *
 * Examples:
 * - export              # Shows all variables
 * - export VAR=value    # Sets VAR to value
 * - export VAR          # Sets VAR to empty string
 * - export A=1 B=2      # Sets multiple variables
 */
int	builtin_export(t_execcmd *ecmd, t_shell *shell)
{
	int		i;
	char	*name;
	char	*value;
	int		ret;

	if (!ecmd->argv[1])
	{
		print_env_vars(shell->env_list);
		return (0);
	}
	i = 1;
	ret = 0;
	while (ecmd->argv[i])
	{
		parse_export_arg(ecmd->argv[i], &name, &value);
		if (!name || !value)
		{
			free(name);
			free(value);
			ret = 1;
			i++;
			continue;
		}
		if (update_or_add_env(shell, name, value) < 0)
			ret = 1;
		free(name);
		free(value);
		i++;
	}
	return (ret);
}
