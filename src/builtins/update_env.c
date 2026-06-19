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
    char *tmp;

    node = find_node(*env, key);
    if (node)
    {
        tmp = ft_strdup(new_value);
        if (!tmp) return (1);
        free(node->value);
        node->value = tmp;
        node->has_equal = 1;
        return (0);
    }
    new = create_env_node(key, new_value);
    if (!new)
        return (1);
    node->has_equal = 1;
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
        ft_putstr_fd("minishe: cd : there are too many arguments\n", 2);
        shell->exit_status = 1;
        return (1);
    }
    if (!args[1])
        path = get_env_value(shell->env, "HOME");
    else
        path = args[1];
    if (!path)
    {
        ft_putstr_fd("minishell : cd : HOME is not defined!\n", 2);
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
// A word consisting solely of letters, numbers, and underscores,
// and beginning with a letter or underscore.
int is_valid_name(char *str, char *func)
{
    int i;

    i = 0;
    if (!str || !str[0])
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(func, 2);
        ft_putstr_fd(": not a valid identifier\n", 2);
        return (0);
    }
    if (!(str[0] == '_' || (str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z')))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(func, 2);
        ft_putstr_fd(": not a valid identifier\n", 2);
        return (0);
    }
    while (str[i])
    {
        if (!(str[i] == '_' || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')))
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(func, 2);
            ft_putstr_fd(": not a valid identifier\n", 2);
            return (0);
        }
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

static char *get_export_key(char *arg)
{
    char *equal;

    equal = ft_strchr(arg, '=');
    if (!equal)
        return (ft_strdup(arg));
    return (ft_substr(arg, 0, equal - arg));
}

static char *get_export_value(char *arg)
{
    char *equal;

    equal = ft_strchr(arg, '=');
    if (!equal)
        return (ft_strdup(""));
    return (ft_strdup(equal + 1));
}

int export_env_func(char **args, t_shell *shell)
{
    //char **split; // split[0] = key, split[1] = value
    t_env *node;
    char *value;
    char *key;
    char *new_value;
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
            has_equal = (ft_strchr(args[i], '=') != NULL);
            key = get_export_key(args[i]);
            value = get_export_value(args[i]);
            if (!key || !value)
            {
                free(key);
                free(value);
                shell->exit_status = 1;
                return (1);
            }
            if (!is_valid_name(key, "export"))
            {
                status = 1;
                free(key);
                free(value);
                i++;
                continue;
            }
            node = find_node(shell->env, key);
            if (node)
            {
                if (has_equal)
                {
                    new_value = ft_strdup(value);
                    if (!new_value)
                    {
                        free(key);
                        free(value);
                        shell->exit_status = 1;
                        return (1);
                    }
                    free(node->value);
                    node->value = new_value;
                    node->has_equal = 1;
                }
            }
            else
            {
                node = create_env_node(key, value);
                if (!node)
                {
                    free(key);
                    free(value);
                    shell->exit_status = 1;
                    return (1);
                }
                node->has_equal = has_equal;
                ft_lstadd_back_env(&shell->env, node);
            }
            free(key);
            free(value);
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
            ft_putstr_fd("minishell : cd : error\n", STDERR_FILENO);
    }
    if (ft_strncmp(cmd->args[0], "export", ft_strlen("export") + 1) == 0)
    {
        if (export_env_func(cmd->args, shell))
            ft_putstr_fd("minishell : export : error\n", STDERR_FILENO);
    }
    if (ft_strncmp(cmd->args[0], "unset", ft_strlen("unset") + 1) == 0)
    {
        if (unset_env_func(cmd->args, shell))
            ft_putstr_fd("minishell : unset : error\n", STDERR_FILENO);
    }
    if (ft_strncmp(cmd->args[0], "env", ft_strlen("env") + 1) == 0)
    {
        if (env_func(shell))
            ft_putstr_fd("minishell : env: error\n", STDERR_FILENO);
    }
}