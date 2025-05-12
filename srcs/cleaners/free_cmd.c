/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:53:29 by mukibrok          #+#    #+#             */
/*   Updated: 2025/04/24 15:38:43 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * free_cmd - Frees a command structure and its components
 * @cmd: Pointer to the command structure to be freed
 *
 * Frees the command structure and its components recursively.
 * Handles different command types (EXEC, REDIR, PIPE, LIST, BACK).
 **/

void free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	// if (cmd->type == EXEC)
	// {
	// 	t_execcmd *ecmd = (t_execcmd *)cmd;
	// 	for (int i = 0; ecmd->argv[i]; i++)
	// 	{
	// 		if (ecmd->argv[i])
	// 			free(ecmd->argv[i]);
	// 	}
	// }
    if (cmd->type == REDIR)
	{
		t_redircmd *rcmd = (t_redircmd *)cmd;
		free_cmd(rcmd->cmd);
		// if (rcmd->file)
		// 	free(rcmd->file);
		// if (rcmd->efile)
		// 	free(rcmd->efile);
	}
	else if (cmd->type == PIPE)
	{
		t_pipecmd *pcmd = (t_pipecmd *)cmd;
		free_cmd(pcmd->left);
		free_cmd(pcmd->right);
	}
	else if (cmd->type == LIST)
	{
		t_listcmd *lcmd = (t_listcmd *)cmd;
		free_cmd(lcmd->left);
		free_cmd(lcmd->right);
	}
	else if (cmd->type == BACK)
	{
		t_backcmd *bcmd = (t_backcmd *)cmd;
		free_cmd(bcmd->cmd);
	}
	free(cmd);
}