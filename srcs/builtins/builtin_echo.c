/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:01 by gansari           #+#    #+#             */
/*   Updated: 2025/05/13 17:51:03 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	is_valid_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	if (str[1] == '\0')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_echo_args(t_execcmd *ecmd, int *n_flag, int *start_idx)
{
	*n_flag = 0;
	*start_idx = 1;
	if (!ecmd->argv[1])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (1);
	}
	if (is_valid_n_flag(ecmd->argv[1]))
	{
		*n_flag = 1;
		*start_idx = 2;
		if (!ecmd->argv[2])
			return (1);
	}
	return (0);
}

static char	*prepare_echo_input(t_execcmd *ecmd, int start_idx)
{
	char	*combined_input;
	char	*temp;

	combined_input = combine_arguments(ecmd->argv, start_idx);
	if (!are_quotes_balanced(combined_input))
	{
		temp = combined_input;
		combined_input = get_continuation_input(combined_input);
		free(temp);
	}
	return (combined_input);
}

static void	output_echo_result(char *processed_output, int n_flag)
{
	ft_putstr_fd(processed_output, STDOUT_FILENO);
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
}

int	builtin_echo(t_execcmd *ecmd, t_shell *shell)
{
	int		n_flag;
	int		start_idx;
	char	*combined_input;
	char	*processed_output;

	(void)shell;
	if (handle_echo_args(ecmd, &n_flag, &start_idx))
		return (0);
	combined_input = prepare_echo_input(ecmd, start_idx);
	processed_output = process_quotes(combined_input);
	output_echo_result(processed_output, n_flag);
	free(combined_input);
	free(processed_output);
	return (0);
}
