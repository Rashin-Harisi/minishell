#include "minishell.h"

int count_node(t_cmd *cmds)
{
    int i;
    t_cmd *tmp;

    tmp = cmds;
    i = 0;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

int is_builtin(char **args)
{
    if (!args || !args[0]) return(0);
    if (ft_strncmp(args[0], "cd", 3) == 0 || ft_strncmp(args[0], "exit", 5) == 0 || ft_strncmp(args[0], "env", 4) == 0 || ft_strncmp(args[0], "export", 7) == 0 || ft_strncmp(args[0], "unset", 6) == 0 || ft_strncmp(args[0], "echo", 5) == 0 || ft_strncmp(args[0], "pwd", 4) == 0)
        return (1);
    return (0);
}

int calculate_nodes(t_env *env)
{
    int i;

    i = 0;
    while (env)
    {
        if (env->has_equal)
            i++;
        env = env->next;
    }
    return (i);
}

char    **convert_list_to_array(t_env *env)
{
    char    **envp;
    t_env   *tmp;
    int     i;
    char    *line;

    i = 0;
    envp = ft_calloc((calculate_nodes(env) + 1) , sizeof(char *));
    if (!envp) return NULL;
    tmp = env;
    while(tmp)
    {
        if (tmp->has_equal)
        {
            line = ft_strjoin(tmp->key, "=");
            if (!line) return (free_array(envp), NULL);
            envp[i] = ft_strjoin(line, tmp->value);
            free(line);
            if (!envp[i]) return (free_array(envp), NULL);
            i++;
        }
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return (envp);
}
