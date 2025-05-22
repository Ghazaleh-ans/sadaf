/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:01:39 by gansari           #+#    #+#             */
/*   Updated: 2025/05/22 13:06:29 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

char	*process_quotes(char *input)
{
	int		i;
	int		j;
	int		quote_state;
	char	*result;

	i = 0;
	j = 0;
	quote_state = 0;
	result = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (!is_quote_char(input[i], &quote_state))
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	are_quotes_balanced(char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (!in_single_quote && !in_double_quote);
}

char	*get_continuation_input(char *initial_input)
{
	char	*line;
	char	*result;
	char	*temp;

	result = ft_strdup(initial_input);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		temp = result;
		result = ft_strjoin(result, "\n");
		free(temp);
		temp = result;
		result = ft_strjoin(result, line);
		free(temp);
		free(line);
		if (are_quotes_balanced(result))
			break ;
	}
	return (result);
}

char	*combine_arguments(char **argv, int start_idx)
{
	int		i;
	char	*combined;
	char	*temp;

	i = start_idx;
	combined = ft_strdup("");
	while (argv[i])
	{
		if (i > start_idx)
		{
			temp = combined;
			combined = ft_strjoin(combined, " ");
			free(temp);
		}
		temp = combined;
		combined = ft_strjoin(combined, argv[i]);
		free(temp);
		i++;
	}
	return (combined);
}

int	is_quote_char(char c, int *quote_state)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = *quote_state & 1;
	in_double_quote = (*quote_state >> 1) & 1;
	if (c == '\'' && !in_double_quote)
	{
		*quote_state ^= 1;
		return (1);
	}
	else if (c == '"' && !in_single_quote)
	{
		*quote_state ^= 2;
		return (1);
	}
	return (0);
}
