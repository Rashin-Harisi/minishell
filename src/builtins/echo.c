#include "minishell.h"

int is_echo_n_option(char *str)
{
    int i;

    if (!str || str[0] != '-' || str[1] != 'n')
        return (0);
    i = 1;
    while (str[i] == 'n')
        i++;
    return (str[i] == '\0');
}

int echo_func(t_shell *shell, t_cmd *cmd)
{
    int i;
    int n_flag;

    i = 1;
    (void)shell;
    n_flag = 0;
    while (cmd->args[i]
        && is_echo_n_option(cmd->args[i])) 
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