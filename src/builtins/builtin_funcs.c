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

int echo_func(t_shell *shell, t_cmd *cmd)
{
    int i;
    int n_flag;

    i = 1;
    (void)shell;
    n_flag = 0;
    if (cmd->args[i]
        && ft_strncmp(cmd->args[i], "-n", 3) == 0) 
    {
        n_flag = 1;
        i++;
    }
    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
    if (!n_flag)
        printf("\n");
    return (0);
}

int exit_func(t_shell *shell, t_token *tokens, t_cmd *cmd)
{
    (void)tokens;
    int code = 0;
    if (cmd->args[0] && !cmd->args[1])
    {
        if (shell->interactive && !shell->in_pipe)
            ft_putstr_fd("exit\n", STDERR_FILENO);
        shell->exit_status = 0;
        return (2);
    }
    if (!is_valid_arg(cmd->args[1]))
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        if (shell->interactive && !shell->in_pipe) 
            ft_putstr_fd("exit\n", STDERR_FILENO);
        shell->exit_status = 2;
        return (2);
    }
    if (cmd->args[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        shell->exit_status = 1;
        return (1);
    }
    if (shell->interactive && !shell->in_pipe)
        ft_putstr_fd("exit\n", STDERR_FILENO);
    code = ft_atoi(cmd->args[1]);
    shell->exit_status = code;
    return (2);
}

int builtin_functions(t_shell *shell, t_token **tokens)
{
    if (!shell || !shell->cmds->args || !shell->cmds->args[0]) return(1);
    if (builtin_update_env(shell, shell->cmds)) return (1);
    if (ft_strncmp(shell->cmds->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
    {
        if (pwd_func(shell))
        {
            ft_putstr_fd("minishell : pwd: error\n", STDERR_FILENO);
            return (1);
        }
    }
    if (ft_strncmp(shell->cmds->args[0], "echo", ft_strlen("echo") + 1) == 0)
    {
        if(echo_func(shell, shell->cmds))
        {
            ft_putstr_fd("minishell : echo: error\n", STDERR_FILENO);
            return (1);
        }
    }
    if (ft_strncmp(shell->cmds->args[0], "exit", ft_strlen("exit") + 1) == 0)
    {
        return (exit_func(shell, *tokens, shell->cmds));
    }
    return (0);
}