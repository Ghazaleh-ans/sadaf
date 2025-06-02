/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:06:33 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 11:06:36 by gansari          ###   ########.fr       */
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

/**
 * process_single_arg - FIXED using expand_var pattern
 * The key insight: Don't replace ecmd->argv[i] if no expansion is needed
 */
int	process_single_arg(t_execcmd *ecmd, t_shell *shell, int i)
{
	char	*tilde_expanded;
	char	*dollar;
	char	*expanded;

	// Step 1: Try tilde expansion
	tilde_expanded = expand_tilde(ecmd->argv[i], shell);
	if (!tilde_expanded)
		return (0);
	
	// Step 2: Check if we have dollar signs to expand
	dollar = ft_strchr(tilde_expanded, '$');
	if (dollar)
	{
		// We have dollar expansion - handle it and clean up tilde_expanded
		expanded = handle_dollar_expansion(tilde_expanded, shell);
		// tilde_expanded is freed inside handle_dollar_expansion
		return (process_expanded_arg(ecmd, expanded, i));
	}
	else
	{
		// No dollar expansion needed
		// CRITICAL FIX: Check if tilde expansion actually changed anything
		if (ft_strcmp(tilde_expanded, ecmd->argv[i]) == 0)
		{
			// No change occurred, free the duplicate and keep original
			free(tilde_expanded);
			return (0);
		}
		else
		{
			// Tilde expansion changed something, use the new value
			ecmd->argv[i] = tilde_expanded;
			return (0);
		}
	}
}
