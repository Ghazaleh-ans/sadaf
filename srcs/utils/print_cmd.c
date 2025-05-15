/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:27:02 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/15 16:57:03 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	print_redir(t_redircmd *rcmd, const char *type_label)
{
	printf("%s command:\n", type_label);
	printf("  file: %s\n", rcmd->file);
	if (type_label[0] != 'H')
	{
		printf("  mode: %d\n", rcmd->mode);
		printf("  fd: %d\n", rcmd->fd);
	}
	print_cmd(rcmd->cmd);
}

void	print_pipe(t_pipecmd *pcmd)
{
	printf("PIPE command:\n");
	printf("  left:\n");
	print_cmd(pcmd->left);
	printf("  right:\n");
	print_cmd(pcmd->right);
}

void	print_list(t_listcmd *lcmd)
{
	printf("LIST command:\n");
	print_cmd(lcmd->left);
	print_cmd(lcmd->right);
}

void	print_back(t_backcmd *bcmd)
{
	printf("BACK command:\n");
	print_cmd(bcmd->cmd);
}

	//t_execcmd	*ecmd;
	//int			i;
	//if (cmd->type == EXEC)
	//{
	//	ecmd = (t_execcmd *)cmd;
	//	printf("EXEC command:\n");
	//	i = 0;
	//	while (ecmd->argv[i])
	//		printf("  argv[%d]: %s\n", i, ecmd->argv[i++]);
	//}

void	print_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ((void)printf("NULL command\n"));
	else if (cmd->type == REDIR)
		print_redir((t_redircmd *)cmd, "REDIR");
	else if (cmd->type == PIPE)
		print_pipe((t_pipecmd *)cmd);
	else if (cmd->type == LIST)
		print_list((t_listcmd *)cmd);
	else if (cmd->type == BACK)
		print_back((t_backcmd *)cmd);
	else if (cmd->type == HEREDOC)
		print_redir((t_redircmd *)cmd, "HEREDOC");
	else
		printf("Unknown command type: %d\n", cmd->type);
}
