/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:05:10 by mukibrok          #+#    #+#             */
/*   Updated: 2025/06/03 19:05:19 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	validate_numeric_argument(char *clean_arg)
{
	int	i;
	int	valid;

	i = 0;
	valid = 1;
	if (!clean_arg[i])
		valid = 0;
	while (clean_arg[i] && valid)
	{
		if (!ft_isdigit(clean_arg[i]))
			valid = 0;
		i++;
	}
	return (valid);
}
