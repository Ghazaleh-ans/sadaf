/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection_utils2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:25:22 by gansari           #+#    #+#             */
/*   Updated: 2025/05/27 13:25:25 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	validate_redirections_left_to_right(t_redircmd **redirections,
	int count)
{
	int	i;

	i = count - 1;
	while (i >= 0)
	{
		if (validate_single_redirection(redirections[i]) < 0)
			return (-1);
		i--;
	}
	return (0);
}

int	validate_all_redirections(t_redircmd *rcmd)
{
	t_redircmd	*redirections[10];
	int			count;

	if (collect_all_redirections(rcmd, redirections, &count) < 0)
		return (-1);
	return (validate_redirections_left_to_right(redirections, count));
}

/*
 * handle_file_redirection - Implements file redirection for shell commands
 * 
 * This function handles both input and output redirections:
 * - Input redirection (<): Opens file and redirects to stdin
 * - Output redirection (>, >>): Opens file and redirects to stdout
 */
int	extract_and_clean_filename(t_redircmd *rcmd,
	char **filename, char **clean_filename)
{
	*filename = ft_substr(rcmd->file, 0, rcmd->efile - rcmd->file);
	if (!*filename)
	{
		ft_putstr_fd("\x1b[31msadaf: memory allocation error\n", STDERR_FILENO);
		return (-1);
	}
	*clean_filename = remove_quotes(*filename);
	if (!*clean_filename)
	{
		free(*filename);
		ft_putstr_fd("\x1b[31msadaf: memory allocation error\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

void	cleanup_filenames(char *filename, char *clean_filename)
{
	free(filename);
	free(clean_filename);
}

int	handle_file_redirection(t_redircmd *rcmd)
{
	int		fd;
	char	*filename;
	char	*clean_filename;
	int		result;

	if (extract_and_clean_filename(rcmd, &filename, &clean_filename) < 0)
		return (-1);
	fd = open_file(clean_filename, rcmd->mode);
	result = setup_file_redirection(fd, rcmd->fd, clean_filename);
	if (result < 0)
	{
		cleanup_filenames(filename, clean_filename);
		return (-1);
	}
	cleanup_filenames(filename, clean_filename);
	return (0);
}
