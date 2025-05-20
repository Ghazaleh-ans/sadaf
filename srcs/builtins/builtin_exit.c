/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:25 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:35:27 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

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

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i++]))
		{
			ft_putstr_fd("sadaf: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			return (2);
		}
	}
	return (-1);
}

/**
 * Processes command line arguments for the exit command.
 *
 * This function handles all argument validation and error cases for exit:
 * 1. Validates that the first argument is numeric (if provided)
 * 2. Converts valid numeric argument to integer
 * 3. Checks for too many arguments (only 0 or 1 argument allowed)
 *
 * Return values:
 * -2: Too many arguments error (shell should not exit)
 * 2: Invalid numeric argument (shell should exit with code 2)
 * Valid integer: The exit code to use
 *
 * @param ecmd The command structure containing arguments
 * @return Exit code to use or error code
 */
static int	handle_exit_args(t_execcmd *ecmd)
{
	int	exit_code;

	exit_code = validate_exit_arg(ecmd->argv[1]);
	if (exit_code != -1)
		return (exit_code);
	exit_code = ft_atoi(ecmd->argv[1]);
	if (ecmd->argv[2])
	{
		ft_putstr_fd("sadaf: exit: too many arguments\n", STDERR_FILENO);
		return (-2);
	}
	return (exit_code);
}

/**
 * Implements the exit builtin command for the shell.
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
	exit(exit_code & 0xFF);
}
