/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:22:19 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/27 13:33:27 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	is_inside_single_quotes(char *arg, int pos)
{
	int		i;
	int		in_single_quotes;

	i = 0;
	in_single_quotes = 0;
	while (i < pos)
	{
		if (arg[i] == '\'' && !is_inside_double_quotes(arg, i))
			in_single_quotes = !in_single_quotes;
		i++;
	}
	return (in_single_quotes);
}

int	is_inside_double_quotes(char *arg, int pos)
{
	int		i;
	int		in_double_quotes;

	i = 0;
	in_double_quotes = 0;
	while (i < pos)
	{
		if (arg[i] == '"' && !is_inside_single_quotes_up_to(arg, i))
			in_double_quotes = !in_double_quotes;
		i++;
	}
	return (in_double_quotes);
}

int	is_inside_single_quotes_up_to(char *arg, int pos)
{
	int		i;
	int		in_single_quotes;

	i = 0;
	in_single_quotes = 0;
	while (i < pos)
	{
		if (arg[i] == '\'')
			in_single_quotes = !in_single_quotes;
		i++;
	}
	return (in_single_quotes);
}

char	*handle_exit_status(char *expanded, t_shell *shell)
{
	char	exit_str[12];
	char	*tmp;

	ft_snprintf(exit_str, sizeof(exit_str), "%d", shell->exit_status);
	tmp = expanded;
	expanded = ft_strjoin(expanded, exit_str);
	free(tmp);
	return (expanded);
}

char	*handle_invalid_var(char *expanded)
{
	char	*tmp;

	tmp = expanded;
	expanded = ft_strjoin(expanded, "$");
	free(tmp);
	return (expanded);
}
