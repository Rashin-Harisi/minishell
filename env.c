#include "minishell.h"

int    extract_key_value(char *str,char **key, char **value)
{
    char    *sign;

    *key = NULL;
    *value = NULL;
    if (!str)
        return (0);
    sign = ft_strchr(str, '=');
    if (!sign)
    {
        *key = ft_strdup(str);
        if (!*key)
            return (0);
        return (1);
    }
    *key = ft_substr(str, 0, sign - str);
    *value = ft_strdup(sign + 1);
    if (!(*key) ||!(*value))
        return (0);
    else
        return (1);
}

void	ft_lstadd_back_env(t_env **env, t_env *new)
{
	t_env	*current;

	if (!env || !new)
		return ;
	if (*env == NULL)
		*env = new;
	else
	{
		current = *env;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

void    free_envs(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        if (env->value)
            free(env->value);
        if (env->key)
            free(env->key);
        free(env);
        env = tmp;
    }
}

t_env   *init_env(char **envp)
{
    t_env   *env;
    t_env   *node;
    int     i;

    i = 0;
    env = NULL;
    while (envp[i] != NULL)
    {
        node = malloc(sizeof(t_env));
        if (!node) 
        {
            free_envs(env);
            return (NULL);
        }
        if (!extract_key_value(envp[i], &node->key, &node->value))
        {
            free(node->key);
            free(node->value);
            free(node);
            free_envs(env);
            return (NULL);
        }
        node->next = NULL;
        ft_lstadd_back_env(&env, node);
        i++;
    }
    return (env);
}