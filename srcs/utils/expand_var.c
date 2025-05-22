/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:11:20 by gansari           #+#    #+#             */
/*   Updated: 2025/05/22 20:15:29 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"
int	is_inside_single_quotes(char *arg, int pos);
int	is_inside_double_quotes(char *arg, int pos);
int	is_inside_single_quotes_up_to(char *arg, int pos);

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

char	*expand_var(char *expanded, char *arg, int *j, t_shell *shell)
{
	int		start;
	char	*var;
	char	*value;
	char	*tmp;

	start = *j + 1;
	if (!arg[start] || (!ft_isalnum(arg[start]) && arg[start] != '_'))
	{
		tmp = expanded;
		expanded = ft_strjoin(expanded, "$");
		free(tmp);
		return (expanded);
	}
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

char	*process_arg(char *arg, t_shell *shell)
{
	int		j;
	char	*expanded;

	expanded = ft_strdup("");
	j = 0;
	while (arg[j])
	{
		if (arg[j] == '$' && !is_inside_single_quotes(arg, j))
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
		dollar = ft_strchr(ecmd->argv[i], '$');
		if (dollar)
		{
			expanded = process_arg(ecmd->argv[i], shell);
			ecmd->argv[i] = expanded;
		}
		i++;
	}
}
