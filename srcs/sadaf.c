/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:28:51 by gansari           #+#    #+#             */
/*   Updated: 2025/05/20 16:43:48 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

static bool	should_skip_command(char *buf)
{
	return (if_only_token(buf)
		|| should_skip_empty_command(buf)
		|| handle_special_command(buf)
		|| if_contains_lparen(buf));
}

void	shell_loop(t_shell *shell)
{
	char	*buf;

	while (1)
	{
		prepare_for_command();
		buf = getcmd();
		if (!buf)
			break ;
		if (should_skip_command(buf))
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
