/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sadaf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:28:51 by gansari           #+#    #+#             */
/*   Updated: 2025/05/26 11:34:13 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sadaf.h"

static int	should_skip_command(char *buf)
{
	int	token_check;

	token_check = 0;
	if (should_skip_empty_command(buf))
		return (1);
	if (token_check == 1)
		return (3);
	else if (token_check == 2)
		return (4);
	token_check = if_only_token(buf);
	if (token_check)
		return (token_check);
	token_check = if_contains_lparen(buf);
	if (token_check)
		return (token_check);
	return (0);
}

void	process_signal(t_shell *shell)
{
	if (g_signal_received)
	{
		shell->exit_status = 130;
		g_signal_received = 0;
	}
}

int	handle_command(char *buf, t_shell *shell)
{
	int	skip_status;

	skip_status = should_skip_command(buf);
	if (skip_status)
	{
		if (skip_status == 3)
			shell->exit_status = 1;
		else if (skip_status == 2)
			shell->exit_status = 2;
		else if (skip_status == 4)
			shell->exit_status = 0;
		return (1);
	}
	execution(buf, shell);
	return (0);
}

void	shell_loop(t_shell *shell)
{
	char	*buf;

	while (1)
	{
		prepare_for_command(shell);
		buf = getcmd();
		process_signal(shell);
		if (!buf)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (handle_command(buf, shell))
		{
			free(buf);
			continue ;
		}
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
	setup_signals(0, shell);
	shell_loop(shell);
	free_shell(shell);
	rl_clear_history();
	return (EXIT_SUCCESS);
}
