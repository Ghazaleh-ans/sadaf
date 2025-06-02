/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:35:27 by gansari           #+#    #+#             */
/*   Updated: 2025/06/01 01:54:17 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sadaf.h"

static int	is_builtin_no_fork(const char *cmd)
{
	return (ft_strcmp((char *)cmd, "cd") == 0
		|| ft_strcmp((char *)cmd, "exit") == 0
		|| ft_strcmp((char *)cmd, "export") == 0
		|| ft_strcmp((char *)cmd, "unset") == 0);
}

static int	should_fork(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	if (cmd->type != EXEC)
		return (1);
	ecmd = (t_execcmd *)cmd;
	return (!is_builtin_no_fork(ecmd->argv[0]));
}

/**
 * handle_exit_command - Special handling for exit command
 */
static int	handle_exit_command(t_execcmd *ecmd, t_shell *shell, t_cmd *cmd)
{
	int		exit_code;
	char	*clean_arg;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	
	// Calculate exit code
	exit_code = shell->exit_status; // default
	if (ecmd->argv[1])
	{
		if (ecmd->argv[2])
		{
			ft_putstr_fd("sadaf: exit: too many arguments\n", STDERR_FILENO);
			return (1); // Don't exit, just return error
		}
		
		clean_arg = remove_quotes(ecmd->argv[1]);
		if (clean_arg)
		{
			// Simple validation - check if it's numeric
			int i = 0;
			int valid = 1;
			if (clean_arg[i] == '+' || clean_arg[i] == '-')
				i++;
			if (!clean_arg[i])
				valid = 0;
			while (clean_arg[i] && valid)
			{
				if (!ft_isdigit(clean_arg[i]))
					valid = 0;
				i++;
			}
			
			if (!valid)
			{
				ft_putstr_fd("sadaf: exit: ", STDERR_FILENO);
				ft_putstr_fd(clean_arg, STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
				exit_code = 2;
			}
			else
			{
				exit_code = ft_atoi(clean_arg);
			}
			free(clean_arg);
		}
	}
	
	// Clean up the command structure
	free_cmd(cmd);
	
	// Clean up shell and exit
	rl_clear_history();
	free_shell(shell);
	exit(exit_code & 0xFF);
}

/**
 * execute_builtin - Execute builtin command (SIMPLIFIED)
 * Memory management is now handled in expand_tilde itself
 */
static void	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	t_execcmd	*ecmd;
	int			status;
	char		*clean_cmd;

	ecmd = (t_execcmd *)cmd;
	expand_variables(ecmd, shell);
	
	// Check if this is an exit command before execution
	clean_cmd = remove_quotes(ecmd->argv[0]);
	if (clean_cmd && ft_strcmp(clean_cmd, "exit") == 0)
	{
		free(clean_cmd);
		handle_exit_command(ecmd, shell, cmd);
		return; // This should never be reached due to exit()
	}
	
	free(clean_cmd);
	status = exec_builtin(ecmd, shell);
	shell->exit_status = status;
}

static void	execute_forked(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	pid;

	pid = protected_fork();
	if (pid == 0)
	{
		setup_signals(1, shell);
		runcmd(cmd, shell);
		free_cmd(cmd);
		exit(EXIT_SUCCESS);
	}
	setup_signals(2, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
	setup_signals(0, shell);
}

void	execution(char *buf, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = parsecmd(buf);
	if (collect_all_heredocs(cmd, shell) < 0)
	{
		free_cmd(cmd);
		return ;
	}
	if (!should_fork(cmd))
	{
		if (cmd->type == EXEC)
			execute_builtin(cmd, shell);
	}
	else
		execute_forked(cmd, shell);
	free_cmd(cmd);
}
