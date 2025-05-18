/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:28:51 by gansari           #+#    #+#             */
/*   Updated: 2025/05/18 22:31:11 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

void	shell_loop(t_shell *shell)
{
	char	*buf;

	while (1)
	{
		prepare_for_command();
		buf = getcmd();
		if (!buf)
			break ;
		if (should_skip_empty_command(buf))
		{
			free(buf);
			continue ;
		}
		if (handle_special_command(buf))
		{
			free(buf);
			continue ;
		}
		execution(buf, shell);
		free(buf);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (perror("Failed to initialize shell"), EXIT_FAILURE);
	setup_signals(0);
	shell_loop(shell);
	free_shell(shell);
	rl_clear_history();
	return (EXIT_SUCCESS);
}
