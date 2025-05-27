/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/27 13:19:41 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	is_only_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

char	*expand_env_vars(char *str)
{
	char	*env_name;
	char	*env_value;

	if (!str || *str != '$')
		return (str);
	if (str[0] == '$')
	{
		env_name = str + 1;
		env_value = getenv(env_name);
		if (!env_value)
			return (str);
		return (env_value);
	}
	return (str);
}

void	prepare_for_command(t_shell *shell)
{
	setup_signals(0, shell);
	g_signal_received = 0;
}

int	should_skip_empty_command(char *buf)
{
	if (is_only_whitespace(buf))
		return (1);
	if (buf[0] == '\n' || buf[0] == '\0')
		return (1);
	return (0);
}
