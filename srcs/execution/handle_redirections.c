/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:23:16 by gansari           #+#    #+#             */
/*   Updated: 2025/06/06 17:10:48 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/*
 * open_file - Opens a file with specified access mode and proper error handling
 */
int	open_file(char *file, int mode)
{
	int	fd;

	if (mode & O_RDONLY)
		fd = open(file, mode);
	else
		fd = open(file, mode, 0644);
	return (fd);
}

/*
 * handle_fd_redirection - Redirects using an already-open file descriptor
 * (Used primarily for heredocs)
 */
static int	handle_fd_redirection(t_redircmd *rcmd)
{
	if (rcmd->fd < 0)
	{
		ft_putstr_fd("Error: Invalid heredoc file descriptor\n", STDERR_FILENO);
		return (-1);
	}
	if (dup2(rcmd->fd, STDIN_FILENO) == -1)
	{
		ft_perror("sadaf: dup2");
		close(rcmd->fd);
		return (-1);
	}
	close(rcmd->fd);
	return (0);
}

/*
 * setup_single_redirection - Handle one redirection
 */
static int	setup_single_redirection(t_redircmd *rcmd)
{
	int	result;

	if (rcmd->file)
		result = handle_file_redirection(rcmd);
	else
		result = handle_fd_redirection(rcmd);
	
	return (result);
}

/*
 * collect_redirections - Collect all redirections into an array
 * This allows us to process them in the correct order (right-to-left)
 */
static int	collect_redirections(t_cmd *cmd, t_redircmd **redirections, int max_count)
{
	int	count = 0;
	
	while (cmd && cmd->type == REDIR && count < max_count)
	{
		redirections[count] = (t_redircmd *)cmd;
		count++;
		cmd = ((t_redircmd *)cmd)->cmd;
	}
	
	return (count);
}

/*
 * FIXED: Process redirections in REVERSE order (right-to-left)
 * This ensures the rightmost redirection takes precedence
 * 
 * For "cat < file1 > file2 > file3":
 * - Process file1 first (input)
 * - Process file2 second (output, but will be overridden)
 * - Process file3 last (final output - this is what actually gets the data)
 */
static int	setup_all_redirections(t_cmd *cmd)
{
	t_redircmd	*redirections[10]; // Support up to 10 redirections
	int			count;
	int			i;
	
	// Collect all redirections
	count = collect_redirections(cmd, redirections, 10);
	
	// Process them in REVERSE order (right-to-left)
	for (i = count - 1; i >= 0; i--)
	{
		if (setup_single_redirection(redirections[i]) < 0)
			return (-1);
	}
	
	return (0);
}

void	handle_redirections(t_redircmd *rcmd, t_shell *shell)
{
	t_cmd	*actual_cmd;

	// Validate all redirections first
	if (validate_all_redirections(rcmd) < 0)
		exit(1);

	// Setup ALL redirections in CORRECT order (right-to-left)
	if (setup_all_redirections((t_cmd *)rcmd) < 0)
		exit(1);

	// Find the actual executable command (skip all REDIR nodes)
	actual_cmd = (t_cmd *)rcmd;
	while (actual_cmd && actual_cmd->type == REDIR)
		actual_cmd = ((t_redircmd *)actual_cmd)->cmd;

	// Execute the actual command with all redirections in place
	runcmd(actual_cmd, shell);
}
