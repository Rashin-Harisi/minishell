#include "minishell.h"

t_env   *init_env(char **envp)
{
    t_env   *env;
    t_env   *node;
    int     i;

    i = 0;
    while (envp[i] != NULL)
    {
        node = malloc(sizeof(t_env));
        node->key
    }
    return (env);
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    shell.env = init_env(envp);

    return (0);
}