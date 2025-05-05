#include "../../includes/sadaf.h"

t_env	*create_env_node(char *name, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	node->value = ft_strdup(value);
	node->next = NULL;
	if (!node->name || !node->value)
	{
		free(node->name);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

void free_env_list(t_env *env_list)
{
    t_env *temp;

    while (env_list)
    {
        temp = env_list;
        env_list = env_list->next;
        free(temp->name);
        free(temp->value);
        free(temp);
    }
}

t_env	*parse_and_create_env_node(char *env_str)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	t_env	*new_node;

	equal_sign = strchr(env_str, '=');
	if (!equal_sign)
		return (NULL);
	name = ft_substr(env_str, 0, equal_sign - env_str);
	if (!name)
		return (NULL);
	value = ft_strdup(equal_sign + 1);
	if (!value) {
		free(name);
		return (NULL);
	}
	new_node = create_env_node(name, value);
	free(name);
	free(value);
	return (new_node);
}

t_env	*append_env_node(t_env *env_list, t_env *new_node)
{
	t_env	*current;

	if (!env_list)
		return (new_node);
	current = env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (env_list);
}

t_env	*parse_env(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = parse_and_create_env_node(envp[i]);
		if (new_node)
			env_list = append_env_node(env_list, new_node);
		i++;
	}
	return (env_list);
}

