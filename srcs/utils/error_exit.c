/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:45:39 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/20 19:59:51 by mukibrok         ###   ########.fr       */
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
		ft_fprintf(2, "\x1b[31mProcess error\n");
	return (pid);
}
