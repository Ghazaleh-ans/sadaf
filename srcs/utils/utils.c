/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:27:25 by codespace         #+#    #+#             */
/*   Updated: 2025/05/26 15:01:45 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

int	if_only_token(const char *str)
{
	char	*ptr;

	ptr = NULL;
	if (!str)
		return (1);
	while (ft_isspace(*str))
		str++;
	ptr = ft_strchr(CHECKER, *str);
	if (ptr)
	{
		ft_fprintf(2, "\x1b[31msadaf: syntax error near unexpected token '%c'\n",
			*ptr);
		return (2);
	}
	return (0);
}

void	ft_error(char *msg)
{
	ft_putstr_fd("\x1b[31msadaf: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	ft_perror(char *msg)
{
	ft_putstr_fd("\x1b[31msadaf: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	protected_fork(void)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_perror("fork");
		exit(1);
	}
	return (pid);
}

void	cleanup_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
