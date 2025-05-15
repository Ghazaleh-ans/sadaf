/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:53:29 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/15 16:04:46 by mukibrok         ###   ########.fr       */
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

void	free_redir_cmd(t_redircmd *rcmd)
{
	if (!rcmd)
		return ;
	free_cmd(rcmd->cmd);
	free(rcmd);
}

void	free_pipe_cmd(t_pipecmd *pcmd)
{
	if (!pcmd)
		return ;
	free_cmd(pcmd->left);
	free_cmd(pcmd->right);
	free(pcmd);
}

void	free_list_cmd(t_listcmd *lcmd)
{
	if (!lcmd)
		return ;
	free_cmd(lcmd->left);
	free_cmd(lcmd->right);
	free(lcmd);
}

void	free_back_cmd(t_backcmd *bcmd)
{
	if (!bcmd)
		return ;
	free_cmd(bcmd->cmd);
	free(bcmd);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == REDIR)
		free_redir_cmd((t_redircmd *)cmd);
	else if (cmd->type == PIPE)
		free_pipe_cmd((t_pipecmd *)cmd);
	else if (cmd->type == LIST)
		free_list_cmd((t_listcmd *)cmd);
	else if (cmd->type == BACK)
		free_back_cmd((t_backcmd *)cmd);
	else
		free(cmd);
}
