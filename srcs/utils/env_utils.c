#include "../../includes/sadaf.h"

void add_env_node(t_shell *shell, t_env *new_node)
{
	t_env *current;

	if (!shell || !new_node)
		return;
	if (!shell->env_list)
	{
		shell->env_list = new_node;
		return;
	}
	current = shell->env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
