#include "../../includes/sadaf.h"

/* Define the global variable for signal handling */
int g_signal_received = 0;

void handle_sigint(int sig)
{
    (void)sig;
    g_signal_received = 1;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigquit(int sig)
{
    (void)sig;
    // Do nothing for SIGQUIT in interactive mode
}

void setup_signals(int mode)
{
    if (mode == 0) // Interactive mode
    {
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, SIG_IGN);
    }
    else if (mode == 1) // Command execution mode
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
    else if (mode == 2) // Heredoc mode
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_IGN);
    }
}