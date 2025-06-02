/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:06:33 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 11:18:10 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

char	*expand_tilde(char *arg, t_shell *shell)
{
	char	*home;
	char	*result;

	if (!arg || arg[0] != '~')
		return (ft_strdup(arg));
	home = ft_getenv("HOME", shell);
	if (!home)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("\x1b[31msadaf: cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
	}
	if (arg[1] == '\0')
		return (ft_strdup(home));
	else if (arg[1] == '/')
	{
		result = ft_strjoin(home, arg + 1);
		return (result);
	}
	else
		return (ft_strdup(arg));
}

char	*handle_dollar_expansion(char *tilde_expanded, t_shell *shell)
{
	char	*expanded;

	expanded = process_arguments(tilde_expanded, shell);
	free(tilde_expanded);
	return (expanded);
}

int	process_expanded_arg(t_execcmd *ecmd, char *expanded, int i)
{
	if (is_empty_after_expansion(expanded))
	{
		free(expanded);
		handle_empty_argv(ecmd, i);
		return (1);
	}
	else
	{
		ecmd->argv[i] = expanded;
		return (0);
	}
}

