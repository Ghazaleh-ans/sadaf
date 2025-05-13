/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:32:47 by gansari           #+#    #+#             */
/*   Updated: 2025/05/13 17:49:40 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

char	*remove_quotes(char *str)
{
	int		len;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 &&
		((str[0] == '\'' && str[len-1] == '\'') ||
			(str[0] == '"' && str[len-1] == '"')))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

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
	char	*raw_value;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		*name = ft_strdup(arg);
		*value = ft_strdup("");
	}
	else
	{
		*name = ft_substr(arg, 0, equal_sign - arg);
		raw_value = ft_strdup(equal_sign + 1);
		*value = remove_quotes(raw_value);
		free(raw_value);
	}
}

static void	process_arg(t_execcmd *ecmd, t_shell *shell, int i, int *ret)
{
	char *name = NULL;
	char *value = NULL;

	parse_export_arg(ecmd->argv[i], &name, &value);
	if (!name || !value)
		*ret = 1;
	else if (update_or_add_env(shell, name, value) < 0)
		*ret = 1;
	free(name);
	free(value);
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
	int i = 1;
	int ret = 0;

	if (!ecmd->argv[1])
	{
		print_env_vars(shell->env_list);
		return (0);
	}
	while (ecmd->argv[i])
		process_arg(ecmd, shell, i++, &ret);
	return (ret);
}
