/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:36:24 by gansari           #+#    #+#             */
/*   Updated: 2025/04/17 12:36:27 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/**
 * @brief Implements the pwd (print working directory) built-in command
 *
 * This function is responsible for printing the current working directory
 * to the standard output. It's a shell built-in equivalent of the Unix pwd command.
 *
 * @param ecmd     Pointer to the execution command structure containing parsed arguments
 * @param shell    Pointer to the shell structure (not used in this function)
 *
 * @return int     Returns 0 on success, 1 on error
 *
 * Function flow:
 * 1. Creates a buffer to store the current directory path
 * 2. Calls getcwd() to retrieve the current working directory
 * 3. Handles error cases when directory can't be obtained
 * 4. Writes the path and a newline to standard output
 *
 * Note: The function ignores the ecmd and shell parameters as they're not needed
 * for this simple command implementation.
 */
int	builtin_pwd(t_execcmd *ecmd, t_shell *shell)
{
	char	buf[2048];

	(void)ecmd;
	(void)shell;
	if (getcwd(buf, sizeof(buf)) == NULL)
	{
		ft_perror("pwd");
		return (1);
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
