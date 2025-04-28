/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:41:55 by gansari           #+#    #+#             */
/*   Updated: 2025/04/24 14:41:58 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	count_env_nodes(t_env *env_list)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(char *name, char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

static int	fill_env_array(char **arr, t_env *env_list)
{
	t_env	*current;
	int		i;

	i = 0;
	current = env_list;
	while (current)
	{
		arr[i] = create_env_string(current->name, current->value);
		if (!arr[i])
			return (0);
		i++;
		current = current->next;
	}
	arr[i] = NULL;
	return (1);
}

char	**env_to_array(t_env *env_list)
{
	char	**arr;
	int		count;

	count = count_env_nodes(env_list);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	if (!fill_env_array(arr, env_list))
	{
		cleanup_tokens(arr);
		return (NULL);
	}
	return (arr);
}
