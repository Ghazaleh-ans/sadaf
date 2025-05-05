#include "../includes/sadaf.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_code;

	if (argc < 1 || !argv || !envp)
	{
		write(2, "Error: Invalid arguments\n", 25);
		return (1);
	}
	if (argc > 1)
	{
		write(2, "Error: Too many arguments\n", 26);
		return (1);
	}
	shell = init_shell(envp);
	if (!shell)
		return (1);
	setup_signals(0);
	exit_code = sadaf_prompt(shell);
	free_shell(shell);
	return (exit_code);
}
