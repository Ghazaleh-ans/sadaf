/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:25:07 by muxammad          #+#    #+#             */
/*   Updated: 2025/05/15 15:59:23 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

void	append_env_node(t_env **head, t_env **tail, t_env *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

t_env	*create_env_node(char *name, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = name;
	node->value = value;
	node->next = NULL;
	return (node);
}

t_env	*parse_and_create_env_node(const char *env_str)
{
	t_env		*new_node;
	char		*equal_sign;
	char		*name;
	char		*value;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign || equal_sign == env_str)
		return (NULL);
	name = ft_substr(env_str, 0, equal_sign - env_str);
	if (!name)
		return (NULL);
	value = ft_strdup(equal_sign + 1);
	if (!value)
		return (free(name), NULL);
	new_node = create_env_node(name, value);
	if (!new_node)
		return (free(name), free(value), NULL);
	return (new_node);
}

t_env	*init_envp(char **envp)
{
	t_env	*new_node;
	t_env	*head;
	t_env	*tail;
	int		i;

	new_node = NULL;
	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = parse_and_create_env_node(envp[i]);
		if (!new_node)
			return (free_env_list(head), NULL);
		append_env_node(&head, &tail, new_node);
		i++;
	}
	return (head);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = init_envp(envp);
	shell->exit_status = 0;
	shell->in_heredoc = 0;
	return (shell);
}
