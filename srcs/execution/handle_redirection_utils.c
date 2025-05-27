/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:19:01 by gansari           #+#    #+#             */
/*   Updated: 2025/05/27 15:00:20 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

/*
 * setup_file_redirection - Sets up file descriptor redirection
 * @fd: Source file descriptor
 * @target_fd: Target file descriptor (0 for stdin, 1 for stdout)
 * @file: Filename for error messages
 * 
 * Returns: 0 on success, -1 on failure
 */
int	setup_file_redirection(int fd, int target_fd, char *file)
{
	if (fd < 0)
	{
		ft_perror(file);
		return (-1);
	}
	if (dup2(fd, target_fd) == -1)
	{
		ft_perror("sadaf: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	collect_all_redirections(t_redircmd *rcmd,
	t_redircmd **redirections, int *count)
{
	t_cmd	*current;
	int		i;

	i = 0;
	current = (t_cmd *)rcmd;
	while (current && i < 10)
	{
		if (current->type == REDIR)
		{
			redirections[i++] = (t_redircmd *)current;
			current = ((t_redircmd *)current)->cmd;
		}
		else
			break ;
	}
	*count = i;
	return (0);
}

/*
 * validate_single_redirection - Validate a single redirection
 * @redir: The redirection to validate
 * 
 * Returns: 0 if valid, -1 if invalid
 */
char	*extract_filename(t_redircmd *redir)
{
	char	*filename;
	char	*clean_filename;

	if (!redir->file)
		return (NULL);
	filename = ft_substr(redir->file, 0, redir->efile - redir->file);
	if (!filename)
		return (NULL);
	clean_filename = remove_quotes(filename);
	free(filename);
	return (clean_filename);
}

int	open_file_with_mode(char *clean_filename, int mode)
{
	int	fd;

	if (mode & O_RDONLY)
		fd = open(clean_filename, mode);
	else
		fd = open(clean_filename, mode, 0644);
	return (fd);
}

int	validate_single_redirection(t_redircmd *redir)
{
	char	*clean_filename;
	int		fd;

	if (!redir->file)
	{
		if (redir->fd < 0)
			return (-1);
		return (0);
	}
	clean_filename = extract_filename(redir);
	if (!clean_filename)
		return (-1);
	fd = open_file_with_mode(clean_filename, redir->mode);
	if (fd < 0)
	{
		ft_perror(clean_filename);
		free(clean_filename);
		return (-1);
	}
	close(fd);
	free(clean_filename);
	return (0);
}
