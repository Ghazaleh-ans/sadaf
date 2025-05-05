/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:01 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:35:05 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * is_valid_n_flag - Checks if a string represents a valid -n flag option
 * @str: The string to check
 *
 * This function validates whether a string is a valid -n option for echo.
 * Valid options include "-n", "-nn", "-nnn", etc. (one or more 'n' after '-')
 *
 * Return: 1 if valid -n flag, 0 otherwise
 */
static int	is_valid_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	if (str[1] == '\0')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * builtin_echo - Implements the echo builtin command
 * @ecmd: Command structure containing arguments to echo
 * @shell: Shell state structure (unused in this function)
 *
 * This function implements echo with support for the -n option.
 * It supports multiple -n flags (e.g., -nnnn) which all have the same effect
 * as a single -n flag. The -n flag suppresses the trailing newline.
 *
 * Examples:
 *   echo hello      -> hello\n
 *   echo -n hello   -> hello
 *   echo -nnnn hi   -> hi
 *   echo -n -nn hi  -> hi
 *
 * Return: Always 0 (success)
 */
int	builtin_echo(t_execcmd *ecmd, t_shell *shell)
{
	int	i;
	int	n_flag;

	(void)shell;
	n_flag = 0;
	i = 1;
	while (ecmd->argv[i] && is_valid_n_flag(ecmd->argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while (ecmd->argv[i])
	{
		ft_putstr_fd(ecmd->argv[i], STDOUT_FILENO);
		if (ecmd->argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
