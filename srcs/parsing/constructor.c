/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:31:17 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/13 17:58:14 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * execcmd - Makes container for regular commands
 *
 * Creates empty command structure for:
 * - Program names (like "ls")
 * - Arguments (like "-la")
 * - No pipes/redirections yet
 *
 * Returns: New command container or NULL on failure
 */

t_cmd	*execcmd(void)
{
	t_execcmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	if (!cmd)
		return (fprintf(stderr, "execcmd: ft_calloc failed"), NULL);
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

/**
 * redircmd - Connects command to input/output files
 * @subcmd: Original command
 * @file: Filename start position
 * @efile: Filename end position
 * @mode: File permissions
 * @fd: File descriptor (0=input, 1=output)
 *
 * Used for:
 * - Input redirection "< file"
 * - Output redirection "> file"
 * - Append mode ">> file"
 *
 * Returns: New command with file connection or NULL on failure
 */

t_cmd	*redircmd(t_cmd *subcmd, t_redirinfo info)
{
	t_redircmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		fprintf(stderr, "redircmd: ft_calloc failed");
		return (free_cmd(subcmd), NULL);
	}
	if (info.heredoc)
		cmd->heredoc = true;
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = info.file;
	cmd->efile = info.efile;
	cmd->mode = info.mode;
	cmd->fd = info.fd;
	return ((t_cmd *)cmd);
}

/**
 * pipecmd - Links two commands with pipe "|"
 * @left: First command (writes to pipe)
 * @right: Second command (reads from pipe)
 *
 * Example: "ls | grep txt" becomes:
 *   pipecmd(ls_command, grep_command)
 *
 * Returns: New pipe command or NULL on failure
 */

t_cmd	*pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		free_cmd(left);
		free_cmd(right);
		return (fprintf(stderr, "pipecmd: ft_calloc failed"), NULL);
	}
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

/**
 * listcmd - Runs commands one after another ";"
 * @left: First command
 * @right: Command to run after first completes
 *
 * Example: "ls; pwd" becomes:
 *   listcmd(ls_command, pwd_command)
 *
 * Returns: New sequence command or NULL on failure
 */

t_cmd	*listcmd(t_cmd *left, t_cmd *right)
{
	t_listcmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		free_cmd(left);
		free_cmd(right);
		return (fprintf(stderr, "listcmd: ft_calloc failed"), NULL);
	}
	cmd->type = LIST;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

/**
 * backcmd - Runs command in background "&"
 * @subcmd: Command to run in background
 *
 * Example: "sleep 10 &" becomes:
 *   backcmd(sleep_command)
 *
 * Returns: New background command or NULL on failure
 */

t_cmd	*backcmd(t_cmd *subcmd)
{
	t_backcmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		free_cmd(subcmd);
		return (fprintf(stderr, "backcmd: ft_calloc failed"), NULL);
	}
	cmd->type = BACK;
	cmd->cmd = subcmd;
	return ((t_cmd *)cmd);
}
