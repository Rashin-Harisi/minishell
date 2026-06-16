#include "minishell.h"

t_env *find_node(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

int update_env_value(t_env **env, char *key, char *new_value)
{
    t_env *node;
    t_env *new;

    node = find_node(*env, key);
    if (node)
    {
        free(node->value);
        node->value = ft_strdup(new_value);
        if (!node->value)
            return (1);
        return (0);
    }
    new = create_env_node(key, new_value);
    if (!new)
        return (1);
    ft_lstadd_back_env(env, new);
    return (0);
}

int cd_env_func(char **args, t_shell *shell)
{
    char *path;
    char old_pwd[PATH_MAX];
    char curr_pwd[PATH_MAX];

    if (args[1] && args[2])
    {
        printf("minishe: cd : there are too many arguments\n");
        shell->exit_status = 1;
        return (1);
    }
    if (!args[1])
        path = get_env_value(shell->env, "HOME");
    else
        path = args[1];
    if (!path)
    {
        printf("minishell : cd : path is not defined!\n");
        shell->exit_status = 1;
        return (1);
    }
    if (!getcwd(old_pwd, sizeof(old_pwd)))
        return (perror("getcwd"), shell->exit_status = 1, 1);
    if (chdir(path) == -1)
        return (perror("cd"), shell->exit_status = 1, 1);
    if (!getcwd(curr_pwd, sizeof(curr_pwd)))
        return (perror("getcwd"), shell->exit_status = 1, 1);
    if (update_env_value(&shell->env, "OLDPWD", old_pwd))
        return (shell->exit_status = 1, 1);
    if (update_env_value(&shell->env, "PWD", curr_pwd))
        return (shell->exit_status = 1, 1);
    shell->exit_status = 0;
    return (0);
}

static void y(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}
// A word consisting solely of letters, numbers, and underscores,
// and beginning with a letter or underscore.
int is_valid_name(char *str, char *func)
{
    int i;

    i = 0;
    if (!(str[0] == '_' || (str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z')))
        return (printf("minishell: %s : not an identifier\n", func), 0);
    while (str[i])
    {
        if (!(str[i] == '_' || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')))
            return (printf("minishell: %s : not an identifier\n", func), 0);
        i++;
    }
    return (1);
}
void    print_export(t_env *env)
{
    while(env)
    {
        if (env->has_equal)
            printf("declare -x %s=\"%s\"\n", env->key, env->value);
        else
            printf("declare -x %s\n", env->key);
        env = env->next;
    }
}

int export_env_func(char **args, t_shell *shell)
{
    char **split; // split[0] = key, split[1] = value
    t_env *node;
    char *value;
    int status = 0;
    int i = 1;
    int has_equal;
    
    has_equal = 0;
    if (!args[1])
        return (print_export(shell->env), shell->exit_status = 0, 0);
    else
    {
        while (args[i])
        {
            if (args[i][0] == '=')
            {
                status = 1;
                i++;
                printf("minishell: export: not a valid identifier\n");
                continue;
            }
            has_equal = (ft_strchr(args[i], '=') != NULL);
            split = ft_split(args[i], '=');
            if (!split || !split[0])
            {
                status = 1;
                i++;
                if (split)
                    y(split);
                continue;
            }
            if (!is_valid_name(split[0], "export"))
            {
                status = 1;
                i++;
                free_array(split);
                continue;
            }
            if (!split[1])
                value = "";
            else
                value = split[1];
            node = find_node(shell->env, split[0]);
            if (node)
            {
                if (has_equal)
                {
                    free(node->value);
                    node->value = ft_strdup(value);
                    if (!node->value)
                        return (free_array(split), shell->exit_status = 1, 1);
                    node->has_equal = 1;
                }
            }
            else
            {
                node = create_env_node(split[0], value);
                if (!node)
                    return (free_array(split), shell->exit_status = 1, 1);
                node->has_equal = has_equal;
                ft_lstadd_back_env(&shell->env, node);
            }
            free_array(split);
            i++;
        }
    }
    shell->exit_status = status;
    return (status);
}

void remove_env_node(t_env **env, t_env *node)
{
    t_env *curr;
    t_env *prev;

    if (!env || !*env || !node)
        return;
    curr = *env;
    prev = NULL;
    while (curr)
    {
        if (ft_strncmp(curr->key, node->key, ft_strlen(node->key) + 1) == 0)
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
    t_env *node;
    int status = 0;

    while (args[i])
    {
        if (!is_valid_name(args[i], "unset"))
        {
            status = 1;
            i++;
            continue;
        }
        node = find_node(shell->env, args[i]);
        if (node)
            remove_env_node(&shell->env, node);
        i++;
    }
    shell->exit_status = status;
    return (status);
}

int env_func(t_shell *shell)
{
    t_env *tmp;

    tmp = shell->env;
    while (tmp)
    {
        if (tmp->has_equal)
            printf("%s=%s\n", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    shell->exit_status = 0;
    return (0);
}

void builtin_update_env(t_shell *shell, t_cmd *cmd)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return;
    if (ft_strncmp(cmd->args[0], "cd", ft_strlen("cd") + 1) == 0)
    {
        if (cd_env_func(cmd->args, shell))
            printf("minishell : cd : error\n");
    }
    if (ft_strncmp(cmd->args[0], "export", ft_strlen("export") + 1) == 0)
    {
        if (export_env_func(cmd->args, shell))
            printf("minishell : export : error\n");
    }
    if (ft_strncmp(cmd->args[0], "unset", ft_strlen("unset") + 1) == 0)
    {
        if (unset_env_func(cmd->args, shell))
            printf("minishell : unset : error\n");
    }
    if (ft_strncmp(cmd->args[0], "env", ft_strlen("env") + 1) == 0)
    {
        if (env_func(shell))
            printf("minishell : env: error\n)");
    }
}