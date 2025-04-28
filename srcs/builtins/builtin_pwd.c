/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:36:24 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:36:27 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	builtin_pwd(t_execcmd *ecmd, t_shell *shell)
{
	char	buf[2048];

	(void)ecmd;
	(void)shell;
	if (getcwd(buf, sizeof(buf)) == NULL)
	{
		ft_perror("pwd");
		return (1);
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
