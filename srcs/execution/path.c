/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:36 by gansari           #+#    #+#             */
/*   Updated: 2025/05/09 13:57:16 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * check_direct_path - Verifies if a command is directly accessible
 *
 * This function checks if a command that contains a path separator ('/')
 * is directly accessible and executable from the specified path.
 *
 * For example, commands like "/bin/ls", "./script.sh", or "../myprog"
 * can be executed directly if they have the executable permission and exist.
 *
 * The function uses access() with X_OK flag to check if the file:
 * 1. Exists at the given path
 * 2. Has executable permission for the current user
 *
 * @param cmd  The command with a direct path to check
 * @return     A duplicated string of the path if executable, otherwise NULL
 *
 * NOTE: The caller must free the returned string to avoid memory leaks.
 */
char	*check_direct_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

/**
 * try_path - Attempts to find an executable in a specific directory
 *
 * This function tries to locate an executable command in a specific directory
 * by combining the directory path with the command name and checking if the
 * resulting path is an executable file.
 *
 * Steps:
 * 1. Builds a complete path by joining base_path + "/" + cmd
 * 2. Checks if the file at that path exists and is executable
 * 3. Returns the complete path if found, NULL otherwise
 *
 * @param base_path  The directory to search in (e.g., "/bin")
 * @param cmd        The command name to look for (e.g., "ls")
 * @return           The complete path if command is found, NULL otherwise
 *
 * NOTE: The caller must free the returned string to avoid memory leaks.
 */
char	*try_path(char *base_path, char *cmd)
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

/**
 * parse_path - Extracts and splits the PATH environment variable
 *
 * This function finds the PATH environment variable in the shell's
 * environment list and splits it into an array of individual directory paths.
 *
 * The PATH variable typically contains a colon-separated list of directories
 * that the shell should search for executable files, e.g.:
 * PATH=/usr/local/bin:/usr/bin:/bin
 *
 * This function:
 * 1. Finds the PATH entry in the environment list
 * 2. Splits the value by ':' separators
 * 3. Returns an array of individual directory paths
 *
 * @param env_list  The shell's environment variable linked list
 * @return          NULL if PATH not found, otherwise array of directory paths
 *
 * NOTE: The caller must free the returned array to avoid memory leaks.
 */
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

/**
 * build_path - Constructs a full path by combining directory and file name
 *
 * This function builds a complete file path by joining a directory path
 * and a command/file name, handling the proper insertion of a slash (/)
 * separator when needed.
 *
 * The function handles two cases:
 * 1. Directory path already ends with '/' - Simply concatenate with the command
 * 2. Directory path doesn't end with '/' - Insert '/' between dir and command
 *
 * @param dir  The directory path (e.g., "/bin" or "/usr/")
 * @param cmd  The command/file name (e.g., "ls")
 * @return     The combined path (e.g., "/bin/ls") or NULL on error
 *
 * NOTE: The caller must free the returned string to avoid memory leaks.
 */
char	*build_path(char *dir, char *cmd)
{
	char	*path;
	char	*tmp;

	if (!dir || !cmd)
		return (NULL);
	if (dir[ft_strlen(dir) - 1] == '/')
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

/**
 * find_command_path - Locates the full path of a command
 *
 * This is the main function for command resolution. It finds the full path
 * to a command by:
 * 1. Handling edge cases (NULL or empty command)
 * 2. Checking if command contains a path separator (/)
 *    - If yes, tries it as a direct path
 * 3. If not, searches each directory in the PATH environment variable
 *    - Parses PATH into individual directories
 *    - Tries each directory to find the command
 *
 * This function implements the standard shell command lookup algorithm
 * where commands are either specified with a path or found in PATH.
 *
 * @param cmd    The command to locate (e.g., "ls" or "/bin/ls")
 * @param shell  The shell structure containing environment variables
 * @return       The full path to the command if found, NULL otherwise
 *
 * NOTE: The caller must free the returned string to avoid memory leaks.
 */
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
