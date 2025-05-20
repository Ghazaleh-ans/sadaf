#include "../../includes/sadaf.h"

char    **allocate_unquoted_array(char **argv, char *path)
{
    int i = 0;
    char **unquoted_argv;

    while (argv[i])
        i++;
    unquoted_argv = malloc(sizeof(char *) * (i + 1));
    if (!unquoted_argv)
    {
        free(path);
        ft_perror("malloc");
        exit(1);
    }
    return (unquoted_argv);
}

void handle_unquote_error(char **unquoted_argv, char *path)
{
    cleanup_tokens(unquoted_argv);
    free(path);
    ft_perror("remove_quotes");
    exit(1);
}

void check_cmd_args(t_execcmd *ecmd, t_shell *shell)
{
    if (!ecmd || !shell)
    {
        ft_error("execute_command: NULL pointer");
        exit(1);
    }
    if (!ecmd->argv[0])
        exit(0);
}