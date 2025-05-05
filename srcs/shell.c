#include "../includes/sadaf.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = parse_env(envp);
	shell->exit_status = 0;
	shell->in_heredoc = 0;
	return (shell);
}

void free_shell(t_shell *shell)
{
    if (!shell)
        return;

    free_env_list(shell->env_list);
    free(shell);
}

int	sadaf_prompt(t_shell *shell)
{
	char	*buf;

	while (1)
	{
		buf = readline("sadaf$ ");
		if (!buf)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			clear_history();
			free_shell(shell);
			break;
		}
		if (*buf)
		{
			add_history(buf);
			process_command(buf, shell);
		}
		free(buf);
		if (g_signal_received)
			g_signal_received = 0;
	}
	return (shell->exit_status);
}

int	process_command(char *buf, t_shell *shell)
{
	t_cmd	*cmd;
	int		pid;
	int		status;

	if (buf[0] == '\0')
		return (0);
	cmd = parsecmd(buf, shell);
	if (!cmd)
		return (1);
	if (cmd->type == EXEC && is_builtin(((t_execcmd *)cmd)->argv[0]))
	{
		status = exec_builtin((t_execcmd *)cmd, shell);
		free_cmd(cmd);
		shell->exit_status = status;
		return (status);
	}
	pid = protected_fork();
	if (pid == 0)
	{
		setup_signals(1);
		runcmd(cmd, shell);
		exit(1);
	}
	setup_signals(2);
	waitpid(pid, &status, 0);
	setup_signals(0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	free_cmd(cmd);
	return (shell->exit_status);
}
