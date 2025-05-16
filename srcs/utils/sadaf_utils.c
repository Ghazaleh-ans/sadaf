/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:38:55 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/16 11:51:20 by gansari          ###   ########.fr       */
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

static int	handle_cd(char *buf)
{
	char	*path;

	if (!buf)
		return (0);
	buf[ft_strcspn(buf, "\n")] = 0;
	if (ft_strncmp(buf, "cd ", 3) == 0 || ft_strcmp(buf, "cd") == 0)
	{
		path = buf + 2;
		while (*path == ' ')
			path++;
		if (*path == 0)
		{
			path = getenv("HOME");
			if (!path)
				path = "/";
		}
		if (chdir(path) < 0)
			fprintf(stderr, "cannot cd to '%s'\n", path);
		return (1);
	}
	return (0);
}

void	prepare_for_command(void)
{
	setup_signals(0);
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

int handle_special_command(char *buf)
{
	return (handle_cd(buf));
}
