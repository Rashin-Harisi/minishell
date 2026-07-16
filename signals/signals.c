# define _POSIX_C_SOURCE 200809L
#include "minishell.h"

volatile sig_atomic_t g_signal = 0;
// sig_atomic_t is a type of integer
/*
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask; The mask contains signals that should be blocked while the handler runs.
    int        sa_flags;
    void     (*sa_restorer)(void);
};
*/

static void handle_prompt_sigint(int sig)
{
    g_signal = sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

static void set_signal_action(int signum, void (*handler)(int))
{
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask); // block nothing
    sa.sa_flags = 0;
    sigaction(signum, &sa, NULL); // install the handler
}

void set_prompt_signals(void)
{
    set_signal_action(SIGINT, handle_prompt_sigint);
    set_signal_action(SIGQUIT, SIG_IGN);
}

void set_parent_wait_signals(void)
{
    set_signal_action(SIGINT, SIG_IGN);
    set_signal_action(SIGQUIT, SIG_IGN);
}

void set_child_signals(void)
{
    set_signal_action(SIGINT, SIG_DFL);
    set_signal_action(SIGQUIT, SIG_DFL);
}