/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:34:44 by gansari           #+#    #+#             */
/*   Updated: 2025/04/24 14:34:49 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void handle_background(t_backcmd *bcmd, t_shell *shell)
{
	int	pid;

	pid = protected_fork();
	if (pid == 0)
		runcmd(bcmd->cmd, shell);
}
