/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:45:39 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/13 18:53:38 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	ft_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		ft_fprintf(2, "Process error\n");
	return (pid);
}
