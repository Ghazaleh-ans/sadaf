/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:25 by gansari           #+#    #+#             */
/*   Updated: 2025/06/01 01:29:06 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	exit_error(char *arg)
{
	ft_putstr_fd("sadaf: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	return (2);
}

/**
 * Validates whether the argument passed to exit is a valid numeric value.
 *
 * This function checks if all characters in the argument are digits.
 * If any non-digit character is found, it prints an error message and
 * returns 2, which is the standard error code for invalid numeric argument.
 *
 * @param arg The string argument to validate
 * @return -1 if the argument is valid (all digits), 2 if invalid
 *
 * Note: The return value of -1 for valid input is used as a flag to
 * indicate that the argument is valid and should be converted to an integer.
 */
static int	validate_exit_arg(char *arg)
{
	int	i;

	if (!arg || !arg[0])
		return (exit_error(arg));
	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (exit_error(arg));
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (exit_error(arg));
		i++;
	}
	return (-1);
}

static char	*remove_all_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

static int	handle_exit_args(t_execcmd *ecmd)
{
	int		exit_code;
	char	*clean_arg;

	clean_arg = remove_all_quotes(ecmd->argv[1]);
	if (!clean_arg)
		return (-2);
	exit_code = validate_exit_arg(clean_arg);
	if (exit_code != -1)
	{
		free(clean_arg);
		return (exit_code);
	}
	exit_code = ft_atoi(clean_arg);
	free(clean_arg);
	if (ecmd->argv[2])
	{
		ft_putstr_fd("sadaf: exit: too many arguments\n", STDERR_FILENO);
		return (-2);
	}
	return (exit_code);
}

/**
 * cleanup_before_exit - Performs comprehensive cleanup before shell exit
 * @shell: Shell structure to clean up
 * 
 * This function ensures all allocated memory is freed before exit:
 * - Clears readline history
 * - Frees environment list
 * - Frees shell structure
 */
static void	cleanup_before_exit(t_shell *shell)
{
	if (shell)
	{
		rl_clear_history();
		free_shell(shell);
	}
	else
		rl_clear_history();
}

/**
 * Implements the exit builtin command for the shell.
 *
 * UPDATED: Now properly cleans up all memory before exiting
 * 
 * The exit command terminates the shell with a specific exit status:
 * - No arguments: Use the last command's exit status
 * - One numeric argument: Use that value as exit status
 * - Invalid argument: Exit with status 2
 * - Too many arguments: Don't exit, return error
 *
 * The exit status is masked with 0xFF to ensure it fits in 8 bits,
 * which is the standard for Unix exit codes (0-255).
 *
 * @param ecmd The command structure containing exit arguments
 * @param shell The shell state structure containing exit_status
 * @return 1 if too many arguments (shell continues), otherwise exits
 *
 * Note: This function prints "exit\n" to STDOUT as a user feedback
 * indicating the shell is terminating.
 */
int	builtin_exit(t_execcmd *ecmd, t_shell *shell)
{
	int	exit_code;

	if (!ecmd || !shell)
		return (1);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!ecmd->argv[1])
		exit_code = shell->exit_status;
	else
	{
		exit_code = handle_exit_args(ecmd);
		if (exit_code == -2)
			return (1);
	}
	cleanup_before_exit(shell);
	exit(exit_code & 0xFF);
}
