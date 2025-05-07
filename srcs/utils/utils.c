#include "../../includes/sadaf.h"

void ft_error(char *msg)
{
    ft_putstr_fd("sadaf: ", STDERR_FILENO);
    ft_putstr_fd(msg, STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

void ft_perror(char *msg)
{
    ft_putstr_fd("sadaf: ", STDERR_FILENO);
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

void *ft_calloc(size_t nmemb, size_t size)
{
    void *ptr;

    ptr = malloc(nmemb * size);
    if (!ptr)
        return (NULL);

    ft_bzero(ptr, nmemb * size);
    return (ptr);
}

void ft_bzero(void *s, size_t n)
{
    memset(s, 0, n);
}

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int ft_isalnum(int c)
{
    return (ft_isalpha(c) || ft_isdigit(c));
}

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

void	cleanup_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void print_cmd(t_cmd *cmd)
{
	if (!cmd)
	{
		printf("NULL command\n");
		return;
	}

	switch (cmd->type)
	{
		case EXEC:
		{
			t_execcmd *ecmd = (t_execcmd *)cmd;
			printf("EXEC command:\n");
			for (int i = 0; ecmd->argv[i]; i++)
				printf("  argv[%d]: %s\n", i, ecmd->argv[i]);
			break;
		}
		case REDIR:
		{
			t_redircmd *rcmd = (t_redircmd *)cmd;
			printf("REDIR command:\n");
			printf("  file: %s\n", rcmd->file);
			printf("  mode: %d\n", rcmd->mode);
			printf("  fd: %d\n", rcmd->fd);
			print_cmd(rcmd->cmd);  // recurse into subcommand
			break;
		}
		case PIPE:
		{
			t_pipecmd *pcmd = (t_pipecmd *)cmd;
			printf("PIPE command:\n");
			printf("  left:\n");
			print_cmd(pcmd->left);
			printf("  right:\n");
			print_cmd(pcmd->right);
			break;
		}
		case LIST:
		{
			t_listcmd *lcmd = (t_listcmd *)cmd;
			printf("LIST command:\n");
			print_cmd(lcmd->left);
			print_cmd(lcmd->right);
			break;
		}
		case BACK:
		{
			t_backcmd *bcmd = (t_backcmd *)cmd;
			printf("BACK command:\n");
			print_cmd(bcmd->cmd);
			break;
		}
		case HEREDOC:
		{
			t_redircmd *rcmd = (t_redircmd *)cmd;
			printf("HEREDOC command:\n");
			printf("  file: %s\n", rcmd->file);
			print_cmd(rcmd->cmd);
			break;
		}
		default:
			printf("Unknown command type: %d\n", cmd->type);
	}
}
