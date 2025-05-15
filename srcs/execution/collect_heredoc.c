/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:14:00 by gansari           #+#    #+#             */
/*   Updated: 2025/05/15 15:49:47 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	collect_all_heredocs(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (cmd->type == REDIR)
		return (collect_heredocs_in_redir(cmd, shell));
	else if (cmd->type == PIPE)
		return (collect_heredocs_in_pipe(cmd, shell));
	else if (cmd->type == LIST)
		return (collect_heredocs_in_list(cmd, shell));
	else if (cmd->type == BACK)
		return (collect_heredocs_in_back(cmd, shell));
	return (0);
}
