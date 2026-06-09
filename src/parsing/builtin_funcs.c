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

int echo_func(t_shell *shell)
{
    int i;
    int n_flag;

    i = 1;
    n_flag = 0;
    if (shell->cmds->args[i]
        && ft_strncmp(shell->cmds->args[i], "-n", 3) == 0) 
    {
        n_flag = 1;
        i++;
    }
    while (shell->cmds->args[i])
    {
        printf("%s\n", shell->cmds->args[i]);
        if (shell->cmds->args[i + 1])
            printf(" ");
        i++;
    }
    if (n_flag)
        printf("\n");
    return (0);
}

int exit_func(t_shell *shell, t_token *tokens)
{
    if (shell->cmds->args[0] && !shell->cmds->args[1])
    {
        free_cmds(shell->cmds);
        free_envs(shell->env);
        free_tokens(tokens);
        printf("exit\n");
        exit(shell->exit_status);
    }
    if (!is_valid_arg(shell->cmds->args[1]))
    {
        printf("minishell: ecit: not valid arguments.\n");
        printf("exit\n");
        free_cmds(shell->cmds);
        free_envs(shell->env);
        free_tokens(tokens);
        exit(2);
    }
    if (shell->cmds->args[2])
    {
        printf("minishell: exit: too many arguments\n");
        shell->exit_status = 1;
        return (1);
    }
    printf("exit\n");
    free_cmds(shell->cmds);
    free_envs(shell->env);
    free_tokens(tokens);
    exit(ft_atoi(shell->cmds->args[1]));
}

void builtin_functions(t_shell *shell, t_token **tokens)
{
    if (!shell || !shell->cmds->args || !shell->cmds->args[0]) return;
    if (builtin_update_env(shell, shell->cmds))
    {
		printf("There is something wrong in updating env\n");
    }
    if (ft_strncmp(shell->cmds->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
    {
        if (pwd_func(shell))
            printf("minishell : pwd: error\n");
    }
    if (ft_strncmp(shell->cmds->args[0], "echo", ft_strlen("echo") + 1) == 0)
    {
        if(echo_func(shell))
            printf("minishell : echo: error\n");
    }
    if (ft_strncmp(shell->cmds->args[0], "exit", ft_strlen("exit") + 1) == 0)
    {
        if(exit_func(shell, *tokens))
            printf("minishell : exit : error\n");
    }
}