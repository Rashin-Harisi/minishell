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
    printf("%s\n", pwd);
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
    int code = 0;
    if (cmd->args[0] && !cmd->args[1])
    {
        free_cmds(shell->cmds);
        free_envs(shell->env);
        free_tokens(tokens);
        printf("exit\n");
        exit(shell->exit_status);
    }
    if (!is_valid_arg(cmd->args[1]))
    {
        printf("minishell: exit: numeric argument required.\n");
        printf("exit\n");
        free_cmds(shell->cmds);
        free_envs(shell->env);
        free_tokens(tokens);
        exit(2);
    }
    if (cmd->args[2])
    {
        printf("minishell: exit: too many arguments\n");
        shell->exit_status = 1;
        return (1);
    }
    printf("exit\n");
    code = ft_atoi(cmd->args[1]);
    free_cmds(shell->cmds);
    free_envs(shell->env);
    free_tokens(tokens);
    exit(code);
}

void builtin_functions(t_shell *shell, t_token **tokens)
{
    if (!shell || !shell->cmds->args || !shell->cmds->args[0]) return;
    builtin_update_env(shell, shell->cmds);
    if (ft_strncmp(shell->cmds->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
    {
        if (pwd_func(shell))
            printf("minishell : pwd: error\n");
    }
    if (ft_strncmp(shell->cmds->args[0], "echo", ft_strlen("echo") + 1) == 0)
    {
        if(echo_func(shell, shell->cmds))
            printf("minishell : echo: error\n");
    }
    if (ft_strncmp(shell->cmds->args[0], "exit", ft_strlen("exit") + 1) == 0)
    {
        if(exit_func(shell, *tokens, shell->cmds))
            printf("minishell : exit : error\n");
    }
}