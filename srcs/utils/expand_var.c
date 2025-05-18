/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:11:20 by gansari           #+#    #+#             */
/*   Updated: 2025/05/15 18:09:40 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

char	*handle_exit_status(char *expanded, t_shell *shell)
{
	char	exit_str[12];
	char	*tmp;

	snprintf(exit_str, sizeof(exit_str), "%d", shell->exit_status);
	tmp = expanded;
	expanded = ft_strjoin(expanded, exit_str);
	free(tmp);
	return (expanded);
}

char	*expand_var(char *expanded, char *arg, int *j, t_shell *shell)
{
	int		start;
	char	*var;
	char	*value;
	char	*tmp;

	start = *j + 1;
	while (arg[*j + 1] && (ft_isalnum(arg[*j + 1]) || arg[*j + 1] == '_'))
		(*j)++;
	if (start <= *j)
	{
		var = ft_substr(arg, start, *j - start + 1);
		value = ft_getenv(var, shell);
		tmp = expanded;
		if (value)
			expanded = ft_strjoin(expanded, value);
		else
			expanded = ft_strjoin(expanded, "");
		free(tmp);
		free(var);
	}
	return (expanded);
}

char	*append_char(char *expanded, char c)
{
	char	tmp_str[2];
	char	*tmp;

	tmp_str[0] = c;
	tmp_str[1] = '\0';
	tmp = expanded;
	expanded = ft_strjoin(expanded, tmp_str);
	free(tmp);
	return (expanded);
}

char	*process_arg(char *arg, t_shell *shell)
{
	int		j;
	char	*expanded;

	expanded = ft_strdup("");
	j = 0;
	while (arg[j])
	{
		if (arg[j] == '$')
		{
			if (arg[j + 1] == '?')
			{
				expanded = handle_exit_status(expanded, shell);
				j += 2;
				continue ;
			}
			expanded = expand_var(expanded, arg, &j, shell);
		}
		else
			expanded = append_char(expanded, arg[j]);
		j++;
	}
	return (expanded);
}

void	expand_variables(t_execcmd *ecmd, t_shell *shell)
{
	int		i;
	char	*dollar;
	char	*expanded;

	i = 0;
	while (ecmd->argv[i])
	{
		dollar = strchr(ecmd->argv[i], '$');
		if (dollar)
		{
			expanded = process_arg(ecmd->argv[i], shell);
			ecmd->argv[i] = expanded;
		}
		i++;
	}
}
