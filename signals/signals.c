# define _POSIX_C_SOURCE 200809L
#include "minishell.h"

// This function is called automatically whenever the shell recieves SIGINT
void    handle_sigint(int sig)
{
    (void)sig;
    
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

/*
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask; The mask contains signals that should be blocked while the handler runs.
    int        sa_flags;
    void     (*sa_restorer)(void);
};
*/

void    init_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask); // block nothing
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL); // install the handler
    signal(SIGQUIT, SIG_IGN); // ignore
}