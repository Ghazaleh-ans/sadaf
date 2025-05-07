/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:36 by gansari           #+#    #+#             */
/*   Updated: 2025/05/06 14:53:14 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static char	*check_direct_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*try_path(char *base_path, char *cmd)
{
	char	*path;

	path = build_path(base_path, cmd);
	if (!path)
		return (NULL);
	if (access(path, X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

char	*find_command_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*path;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (check_direct_path(cmd));
	paths = parse_path(shell->env_list);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = try_path(paths[i], cmd);
		if (path)
		{
			cleanup_tokens(paths);
			return (path);
		}
		i++;
	}
	cleanup_tokens(paths);
	return (NULL);
}

char	**parse_path(t_env *env_list)
{
	t_env	*current;
	char	**paths;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, "PATH") == 0)
			break ;
		current = current->next;
	}
	if (!current)
		return (NULL);
	paths = ft_split(current->value, ':');
	return (paths);
}

char	*build_path(char *dir, char *cmd)
{
	char	*path;
	char	*tmp;

	if (!dir || !cmd)
		return (NULL);
	if (dir[strlen(dir) - 1] == '/')
		path = ft_strjoin(dir, cmd);
	else
	{
		tmp = ft_strjoin(dir, "/");
		if (!tmp)
			return (NULL);
		path = ft_strjoin(tmp, cmd);
		free(tmp);
	}
	return (path);
}
