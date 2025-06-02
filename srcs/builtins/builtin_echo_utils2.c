/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:01:07 by gansari           #+#    #+#             */
/*   Updated: 2025/05/29 16:04:27 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	handle_signal_interrupt(char *result)
{
	free(result);
	printf("\n");
	exit(130);
}

void	handle_eof_input(char *result)
{
	free(result);
	printf("\n");
	exit(2);
}

char	*append_line_to_result(char *result, char *line)
{
	char	*temp;
	char	*with_newline;
	char	*final_result;

	temp = result;
	with_newline = ft_strjoin(result, "\n");
	free(temp);
	temp = with_newline;
	final_result = ft_strjoin(with_newline, line);
	free(temp);
	return (final_result);
}

void	process_input_line(char *result, char *line)
{
	if (g_signal_received == SIGINT)
		handle_signal_interrupt(result);
	else if (!line)
		handle_eof_input(result);
}
