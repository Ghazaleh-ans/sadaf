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

int	builtin_echo(t_execcmd *ecmd, t_shell *shell)
{
	int	i;
	int	n_flag;

	(void)shell;
	n_flag = 0;
	i = 1;
	if (ecmd->argv[1] && ft_strcmp(ecmd->argv[1], "-n") == 0)
	{
		n_flag = 1;
		i = 2;
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
