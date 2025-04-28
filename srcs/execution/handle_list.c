/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:22:41 by gansari           #+#    #+#             */
/*   Updated: 2025/04/24 14:22:43 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	handle_list(t_listcmd *lcmd, t_shell *shell)
{
	int	pid;
	int	status;

	pid = protected_fork();
	if (pid == 0)
		runcmd(lcmd->left, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	runcmd(lcmd->right, shell);
}
