#include "minishell.h"

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
            if (!is_valid_name(key))
            {
                print_identifier_error("export", key, value);
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
    return (0);
}