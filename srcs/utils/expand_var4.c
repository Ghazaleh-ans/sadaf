/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:18:29 by gansari           #+#    #+#             */
/*   Updated: 2025/06/02 11:18:42 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	handle_no_dollar_expansion(t_execcmd *ecmd, char *tilde_expanded, int i)
{
	if (ft_strcmp(tilde_expanded, ecmd->argv[i]) == 0)
	{
		free(tilde_expanded);
		return (0);
	}
	else
	{
		ecmd->argv[i] = tilde_expanded;
		return (0);
	}
}

static int	handle_dollar_found(t_execcmd *ecmd, char *tilde_expanded, 
							t_shell *shell, int i)
{
	char	*expanded;

	expanded = handle_dollar_expansion(tilde_expanded, shell);
	return (process_expanded_arg(ecmd, expanded, i));
}

static int	process_tilde_result(t_execcmd *ecmd, char *tilde_expanded, 
							t_shell *shell, int i)
{
	char	*dollar;

	dollar = ft_strchr(tilde_expanded, '$');
	if (dollar)
	{
		return (handle_dollar_found(ecmd, tilde_expanded, shell, i));
	}
	else
	{
		return (handle_no_dollar_expansion(ecmd, tilde_expanded, i));
	}
}

int	process_single_arg(t_execcmd *ecmd, t_shell *shell, int i)
{
	char	*tilde_expanded;

	tilde_expanded = expand_tilde(ecmd->argv[i], shell);
	if (!tilde_expanded)
		return (0);
	return (process_tilde_result(ecmd, tilde_expanded, shell, i));
}
