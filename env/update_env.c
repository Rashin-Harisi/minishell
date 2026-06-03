#include "minishell.h"

int cd_env_func(char **args, t_env *env)
{
    char    *path;

    if (!args[1])
        path = get_env_value(env, "HOME");
    else
        path = args[1];
    if (!path)
    {
        printf("minishell : cd : path is not defined!");
        return (1);
    }
    if (chdir(path) == -1)
    {
        perror("cd");
        return (1);
    }
    return (0);
}

t_env   *find_node(t_env *env, char *key)
{
    while(env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

void    free_array(char **arr)
{
    int i = 0;
    while(arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int export_env_func(char **args, t_env **env)
{
    char    **split; // split[0] = key, split[1] = value
    t_env   *node;
    char    *value;
    int i = 1;

    if (!args[1])
        return (print_envs(*env), 0);
    else
    {
        while(args[i])
        {
            split = ft_split(args[i], '=');
            if (!split || !split[0])
                return (1);
            if (!split[1]) 
                value = "";
            else
                value = split[1];
            node = find_node(*env, split[0]);
            if (node)
            {
                free(node->value);
                node->value = ft_strdup(value);
                if(!node->value)
                    return (free_array(split), 1);
            }
            else
            {
                node = create_env_node(split[0],value);
                if (!node)
                    return (free_array(split), 1);
                ft_lstadd_back_env(env,node);
            }
            free_array(split);
            i++;
        }
    }
    return (0);
}

void    remove_env_node(t_env **env, t_env *node)
{
    t_env   *curr;
    t_env   *prev;

    if (!env || !*env || !node)
        return;
    curr = *env;
    prev = NULL;
    while (curr)
    {
        if (ft_strncmp(curr->key , node->key, ft_strlen(node->key) + 1) == 0)
        {
            if (prev)
                prev->next = curr->next;
            else
                *env = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int unset_env_func(char **args, t_shell *shell)
{
    int i = 1;
    t_env   *node;

    while (args[i])
    {
        node = find_node(shell->env, args[i]);
        if (node)
            remove_env_node(&shell->env, node);
        i++;
    }
    shell->exit_status = 0;
    return (0);
}

int env_func(t_env *env)
{
    print_envs(env);
    return (0);
}

int builtin_update_env(t_shell *shell, t_cmd *cmd)
{    
    if(!cmd || !cmd->args || !cmd->args[0])
        return (1);
    if (ft_strncmp(cmd->args[0], "cd", ft_strlen("cd") + 1) == 0)
        return (cd_env_func(cmd->args, shell));
    if (ft_strncmp(cmd->args[0], "export", ft_strlen("export") + 1) == 0)
        return (export_env_func(cmd->args, &shell->env));
    if (ft_strncmp(cmd->args[0], "unset", ft_strlen("unset") + 1 ) == 0)
        return (unset_env_func(cmd->args, shell));
    if (ft_strncmp(cmd->args[0], "env", ft_strlen("env") + 1) == 0)
        return (env_func(shell->env));
    return (0);
}