/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:06:00 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 12:06:26 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	validate_numeric_argument(char *clean_arg)
{
	int	i;
	int	valid;

	i = 0;
	valid = 1;
	if (!clean_arg[i])
		valid = 0;
	while (clean_arg[i] && valid)
	{
		if (!ft_isdigit(clean_arg[i]))
			valid = 0;
		i++;
	}
	return (valid);
}

static char	*process_sign_and_quotes(char *clean_arg, int *minus_sign)
{
	int		i;
	char	*result;

	i = 0;
	*minus_sign = 0;
	if (clean_arg[i] == '-')
	{
		*minus_sign = 1;
		i++;
		result = remove_quotes(clean_arg + 1);
		return (result);
	}
	if (clean_arg[i] == '+')
	{
		i++;
		result = remove_quotes(clean_arg + 1);
		return (result);
	}
	return (clean_arg);
}

static void	print_numeric_error(char *original_arg)
{
	ft_putstr_fd("sadaf: exit: ", STDERR_FILENO);
	ft_putstr_fd(original_arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

static int	calculate_exit_code(char *clean_arg, int minus_sign)
{
	int	exit_code;

	if (minus_sign)
		exit_code = 256 + (-(ft_atoi(clean_arg))) % 256;
	else
		exit_code = ft_atoi(clean_arg);
	return (exit_code);
}

static int	parse_exit_argument(char *arg, t_shell *shell)
{
	char	*clean_arg;
	char	*processed_arg;
	int		minus_sign;
	int		exit_code;

	clean_arg = remove_quotes(arg);
	if (!clean_arg)
		return (shell->exit_status);
	processed_arg = process_sign_and_quotes(clean_arg, &minus_sign);
	if (!validate_numeric_argument(processed_arg))
	{
		print_numeric_error(clean_arg);
		free(clean_arg);
		if (processed_arg != clean_arg)
			free(processed_arg);
		return (2);
	}
	exit_code = calculate_exit_code(processed_arg, minus_sign);
	free(clean_arg);
	if (processed_arg != clean_arg)
		free(processed_arg);
	return (exit_code);
}

int	handle_exit_command(t_execcmd *ecmd, t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit_code = shell->exit_status;
	if (ecmd->argv[1])
	{
		if (ecmd->argv[2])
		{
			ft_putstr_fd("sadaf: exit: too many arguments\n", STDERR_FILENO);
			shell->exit_status = 1;
			return (1);
		}
		exit_code = parse_exit_argument(ecmd->argv[1], shell);
	}
	free_cmd(cmd);
	rl_clear_history();
	free_shell(shell);
	exit(exit_code & 0xFF);
}
