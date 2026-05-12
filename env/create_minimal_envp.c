#include "minishell.h"


static t_env    *create_node(char *key, char *value)
{
    t_env   *node;
    node = malloc(sizeof(t_env));
    if (!node) 
        return (NULL);
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
    node->next = NULL;
    if (!node->key || !node->value)
    {
        free(node->key);
        free(node->value);
        free(node);
        return (NULL);
    }
    return (node);
}

static int add_node_to_env(t_env **env, char *key, char *value)
{
    t_env   *node;
    node = create_node(key, value);
    if (!node) return (0);
    ft_lstadd_back_env(env, node);
    return (1);
}

t_env   *create_minimal_envp(void)
{
    t_env   *env;
    char    cwd[PATH_MAX];

    env = NULL;
    if (!add_node_to_env(&env, "PATH", "/usr/local/sbin:/usr/local/bin:/usr/bin:/bin"))
        return (free_envs(env), NULL);
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd() error");
        free_envs(env);
        retrun (NULL);
    }
    if (!add_node_to_env(&env, "PWD", cwd))
        return (free_envs(env), NULL);
    return (env);
}