/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:22:14 by gansari           #+#    #+#             */
/*   Updated: 2025/06/05 11:06:41 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static char	**save_original_argv(t_execcmd *ecmd)
{
	char	**original_argv;
	int		i;

	original_argv = malloc(sizeof (char *) * MAXARGS);
	if (!original_argv)
		return (NULL);
	i = 0;
	while (i < MAXARGS && ecmd->argv[i])
	{
		original_argv[i] = ecmd->argv[i];
		i++;
	}
	while (i < MAXARGS)
	{
		original_argv[i] = NULL;
		i++;
	}
	return (original_argv);
}

static void	free_expanded_argv(t_execcmd *ecmd, char **original_argv)
{
	int	i;

	i = 0;
	while (i < MAXARGS && ecmd->argv[i])
	{
		if (ecmd->argv[i] != original_argv[i])
			free(ecmd->argv[i]);
		i++;
	}
	free(original_argv);
}

static int	handle_exit_builtin(t_execcmd *ecmd, t_shell *shell,
	t_cmd *cmd, char **original_argv)
{
	char	*clean_cmd;

	clean_cmd = remove_quotes(ecmd->argv[0]);
	if (clean_cmd && ft_strcmp(clean_cmd, "exit") == 0)
	{
		free(clean_cmd);
		free_expanded_argv(ecmd, original_argv);
		handle_exit_command(ecmd, shell, cmd);
		return (1);
	}
	free(clean_cmd);
	return (0);
}

void	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	t_execcmd	*ecmd;
	int			status;
	char		**original_argv;
	int			is_exit;

	ecmd = (t_execcmd *)cmd;
	original_argv = save_original_argv(ecmd);
	if (!original_argv)
		return ;
	expand_variables(ecmd, shell);
	is_exit = handle_exit_builtin(ecmd, shell, cmd, original_argv);
	if (is_exit)
		return ;
	status = exec_builtin(ecmd, shell);
	shell->exit_status = status;
	free_expanded_argv(ecmd, original_argv);
}
