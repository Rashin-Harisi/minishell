#include "minishell.h"

int pwd_func(t_shell *shell)
{
    (void)shell;
    char    pwd[PATH_MAX];

    if (!getcwd(pwd, sizeof(pwd)))
        return (perror("getcwd"),1);
    ft_putstr_fd(pwd, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}