/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:28:38 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:28:41 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static char	*get_cd_path(t_execcmd *ecmd, t_shell *shell)
{
	char	*path;

	if (!ecmd->argv[1])
		path = ft_getenv("HOME", shell);
	else
		path = ecmd->argv[1];
	if (!path)
	{
		ft_putstr_fd("sadaf: cd: HOME not set\n", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

static int	change_directory(char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("sadaf: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_perror("");
		return (1);
	}
	return (0);
}

static int	get_pwd(char *buf, size_t size)
{
	char	*pwd;

	pwd = getcwd(buf, size);
	if (!pwd)
	{
		ft_perror("cd");
		return (1);
	}
	return (0);
}

static void	update_env_variables(t_shell *shell, char *current_pwd,
		char *old_pwd)
{
	t_env	*current;

	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->name, "PWD") == 0)
		{
			free(current->value);
			current->value = ft_strdup(current_pwd);
		}
		else if (ft_strcmp(current->name, "OLDPWD") == 0)
		{
			free(current->value);
			current->value = ft_strdup(old_pwd);
		}
		current = current->next;
	}
}

int	builtin_cd(t_execcmd *ecmd, t_shell *shell)
{
	char	*path;
	char	old_buf[2048];
	char	current_buf[2048];

	path = get_cd_path(ecmd, shell);
	if (!path)
		return (1);
	if (get_pwd(old_buf, sizeof(old_buf)))
		return (1);
	if (change_directory(path))
		return (1);
	if (get_pwd(current_buf, sizeof(current_buf)))
		return (1);
	update_env_variables(shell, current_buf, old_buf);
	return (0);
}
