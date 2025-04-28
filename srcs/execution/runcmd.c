/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:39:44 by gansari           #+#    #+#             */
/*   Updated: 2025/04/24 14:39:46 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	runcmd(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		exit(0);
	if (cmd->type == EXEC)
		execute_command((t_execcmd *)cmd, shell);
	else if (cmd->type == REDIR)
		handle_redirections((t_redircmd *)cmd, shell);
	else if (cmd->type == PIPE)
		handle_pipe((t_pipecmd *)cmd, shell);
	else if (cmd->type == LIST)
		handle_list((t_listcmd *)cmd, shell);
	else if (cmd->type == BACK)
		handle_background((t_backcmd *)cmd, shell);
	else
	{
		ft_error("Unknown command type");
		exit(1);
	}
	exit(shell->exit_status);
}
