/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:26:06 by gansari           #+#    #+#             */
/*   Updated: 2025/05/18 21:28:42 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	free_exec_cmd(t_execcmd *ecmd)
{
	int	i;

	if (!ecmd)
		return;
	i = 0;
	while (ecmd->argv[i])
	{
		if (ecmd->argv[i] != ecmd->eargv[i] &&
			ecmd->argv[i] < ecmd->eargv[i])
			free(ecmd->argv[i]);
		i++;
	}
	free(ecmd);
}
