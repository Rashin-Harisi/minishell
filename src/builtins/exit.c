#include "minishell.h"

int is_valid_arg(char *str)
{
    int i;

    i = 0;
    if (!str || !str[0]) return (0);
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (!str[i]) return (0);
    while (str[i] != '\0')
    {
        if (!(str[i] >= '0' && str[i] <= '9')) return (0);
        i++;
    }
    return (1);
}

int exit_func(t_shell *shell, t_token *tokens, t_cmd *cmd)
{
    (void)tokens;
    int code = 0;
    if (cmd->args[0] && !cmd->args[1])
    {
        if (shell->interactive && !shell->in_pipe)
            ft_putstr_fd("exit\n", STDERR_FILENO);
        return (2);
    }
    if (!is_valid_arg(cmd->args[1]))
    {
        ft_putstr_fd("exit: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        if (shell->interactive && !shell->in_pipe) 
            ft_putstr_fd("exit\n", STDERR_FILENO);
        shell->exit_status = 2;
        return (2);
    }
    if (cmd->args[2])
    {
        ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
        shell->exit_status = 1;
        return (1);
    }
    if (shell->interactive && !shell->in_pipe)
        ft_putstr_fd("exit\n", STDERR_FILENO);
    code = ft_atoi(cmd->args[1]);
    shell->exit_status = code;
    return (2);
}