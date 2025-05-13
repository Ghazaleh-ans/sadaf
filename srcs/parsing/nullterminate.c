/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nullterminate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:38:28 by codespace         #+#    #+#             */
/*   Updated: 2025/05/13 16:09:51 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	nulterminate_exec(t_execcmd *ecmd)
{
	int	i;

	i = 0;
	while (ecmd->argv[i])
	{
		*ecmd->eargv[i] = '\0';
		i++;
	}
}

void	nulterminate_redir(t_redircmd *rcmd)
{
	nulterminate(rcmd->cmd);
	*rcmd->efile = '\0';
}

void	nulterminate_pipe(t_pipecmd *pcmd)
{
	nulterminate(pcmd->left);
	nulterminate(pcmd->right);
}

void	nulterminate_list(t_listcmd *lcmd)
{
	nulterminate(lcmd->left);
	nulterminate(lcmd->right);
}

void	nulterminate_back(t_backcmd *bcmd)
{
	nulterminate(bcmd->cmd);
}
