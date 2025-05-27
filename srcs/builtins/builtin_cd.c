/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:28:38 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 12:10:46 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * get_cd_path - Determines the target directory for the cd command
 *
 * This function decides where the cd command should go:
 * - If no argument is provided, uses the HOME environment variable  
 * - If more than one argument is provided, shows "too many arguments" error
 * - If one argument contains spaces, shows "too many arguments" error
 *
 * @param ecmd The execution command structure containing arguments
 * @param shell The shell state containing environment variables
 * @return The target path as a string, or NULL if error
 */
static char	*get_cd_path(t_execcmd *ecmd, t_shell *shell)
{
	char	*path;

	if (!ecmd->argv[1])
	{
		path = ft_getenv("HOME", shell);
		if (!path)
		{
			ft_putstr_fd("\x1b[31msadaf: cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
		return (path);
	}
	if (ecmd->argv[2])
	{
		ft_putstr_fd("\x1b[31msadaf: cd: too many arguments\n", STDERR_FILENO);
		return (NULL);
	}
	if (ft_strchr(ecmd->argv[1], ' '))
	{
		ft_putstr_fd("\x1b[31msadaf: cd: too many arguments\n", STDERR_FILENO);
		return (NULL);
	}
	return (ecmd->argv[1]);
}

/**
 * change_directory - Attempts to change to the specified directory
 *
 * Uses the chdir() system call to change the current working directory.
 * Provides appropriate error messages if the change fails.
 *
 * @param path The directory to change to
 * @return 0 on success, 1 on failure
 */
static int	change_directory(char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("\x1b[31msadaf: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_perror("");
		return (1);
	}
	return (0);
}

/**
 * get_pwd - Gets the current working directory
 *
 * Uses getcwd() to get the current directory path. This is used to save
 * the current directory before changing, so it can be stored in OLDPWD.
 *
 * @param buf Buffer to store the directory path
 * @param size Size of the buffer
 * @return 0 on success, 1 on failure
 */
static int	get_pwd(char *buf, size_t size)
{
	char	*pwd;

	pwd = getcwd(buf, size);
	if (!pwd)
	{
		ft_perror("\x1b[31mcd");
		return (1);
	}
	return (0);
}

/**
 * update_env_variables - Updates PWD and OLDPWD environment variables
 *
 * After successfully changing directories, this function updates:
 * - PWD: Set to the new current directory
 * - OLDPWD: Set to the previous directory (before the change)
 *
 * This allows users to track directory changes and use commands like 'cd -'
 *
 * @param shell The shell state containing environment variables
 * @param current_pwd The new current directory path
 * @param old_pwd The previous directory path
 */
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

/**
 * builtin_cd - Implementation of the cd (change directory) command
 *
 * This is the main function that implements the cd command behavior.
 * The process follows these steps:
 * 1. Determine target directory (from argument or HOME)
 * 2. Check for too many arguments (including expanded variables with spaces)
 * 3. Save current directory as OLDPWD
 * 4. Change to the new directory
 * 5. Update environment variables (PWD, OLDPWD)
 *
 * @param ecmd The execution command structure
 * @param shell The shell state
 * @return 0 on success, 1 on failure
 */
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
