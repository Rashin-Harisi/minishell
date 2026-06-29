#include "minishell.h"

int builtin_functions(t_shell *shell, t_token **tokens)
{
    if (!shell || !shell->cmds->args || !shell->cmds->args[0]) return(1);
    if (builtin_update_env(shell, shell->cmds)) return (1);
    if (ft_strncmp(shell->cmds->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
    {
        if (pwd_func(shell))
        {
            ft_putstr_fd("pwd: error\n", STDERR_FILENO);
            return (1);
        }
    }
    if (ft_strncmp(shell->cmds->args[0], "echo", ft_strlen("echo") + 1) == 0)
    {
        if(echo_func(shell, shell->cmds))
        {
            ft_putstr_fd("echo: error\n", STDERR_FILENO);
            return (1);
        }
    }
    if (ft_strncmp(shell->cmds->args[0], "exit", ft_strlen("exit") + 1) == 0)
    {
        return (exit_func(shell, *tokens, shell->cmds));
    }
    return (0);
}