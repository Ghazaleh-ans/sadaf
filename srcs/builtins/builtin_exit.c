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

/* returning -1 indicates valid number*/
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

/* -2 indicates too many args error (don't exit)*/
static int	handle_exit_args(t_execcmd *ecmd)
{
	int	exit_code;

	exit_code = validate_exit_arg(ecmd->argv[1]);
	if (exit_code != -1)
		return (exit_code);
	exit_code = atoi(ecmd->argv[1]);
	if (ecmd->argv[2])
	{
		ft_putstr_fd("sadaf: exit: too many arguments\n", STDERR_FILENO);
		return (-2);
	}
	return (exit_code);
}

/* exit_code = -2 -> too many args*/
int	builtin_exit(t_execcmd *ecmd, t_shell *shell)
{
	int	exit_code;

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
