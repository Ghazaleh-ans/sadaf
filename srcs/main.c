#include "../includes/sadaf.h"

int main(int argc, char **argv, char **envp)
{
    t_shell *shell;
    int exit_code;
    
    (void)argc;
    (void)argv;
    
    shell = init_shell(envp);
    if (!shell)
        return (1);
    
    setup_signals(0);
    exit_code = sadaf_prompt(shell);
    
    free_shell(shell);
    return (exit_code);
}