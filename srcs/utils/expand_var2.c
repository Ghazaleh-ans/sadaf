/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:24:50 by mukibrok          #+#    #+#             */
/*   Updated: 2025/05/27 13:46:15 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static char	*extract_and_expand(t_util util, int start, int end, t_shell *shell)
{
	char	*var;
	char	*value;
	char	*tmp;

	var = ft_substr(util.arg, start, end - start + 1);
	value = ft_getenv(var, shell);
	tmp = util.expanded;
	if (value)
		util.expanded = ft_strjoin(util.expanded, value);
	else
		util.expanded = ft_strjoin(util.expanded, "");
	free(tmp);
	free(var);
	return (util.expanded);
}

static void	find_var_end(char *arg, int *j)
{
	while (arg[*j + 1] && (ft_isalnum(arg[*j + 1]) || arg[*j + 1] == '_'))
		(*j)++;
}

char	*expand_var(char *expanded, char *arg, int *j, t_shell *shell)
{
	int		start;
	t_util	util;

	start = *j + 1;
	if (!arg[start] || (!ft_isalnum(arg[start]) && arg[start] != '_'))
		return (handle_invalid_var(expanded));
	find_var_end(arg, j);
	if (start <= *j)
	{
		util.expanded = expanded;
		util.arg = arg;
		expanded = extract_and_expand(util, start, *j, shell);
	}
	return (expanded);
}
