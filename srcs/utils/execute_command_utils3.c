/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:51:00 by gansari           #+#    #+#             */
/*   Updated: 2025/05/27 13:35:10 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	is_empty_or_whitespace(char *str)
{
	int	i;

	if (!str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_complex_command(char *cmd_no_quotes)
{
	return (ft_strchr(cmd_no_quotes, ' ') && cmd_no_quotes[0] != '/'
		&& !(cmd_no_quotes[0] == '.' && cmd_no_quotes[1] == '/')
		&& !(cmd_no_quotes[0] == '.' && cmd_no_quotes[1] == '.'
			&& cmd_no_quotes[2] == '/'));
}
