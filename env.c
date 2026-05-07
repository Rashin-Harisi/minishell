#include "minishell.h"

void    extract_key_value(char *str,char **key, char **value)
{
    char    *sign;

    if (!str)
    {
        *key = NULL;
        *value = NULL;
        return;
    }
    sign = ft_strchr(str, '=');
    if (!sign)
    {
        *key = ft_strdup(str);
        *value = NULL;
        return;
    }
    *key = ft_substr(str, 0, sign - str);
    *value = ft_strdup(sign + 1);
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
        if (!node) return (NULL);
        extract_key_value(envp[i], &node->key, &node->value);
        node->next = NULL;
        ft_lstadd_back_env(&env, node);
        i++;
    }
    return (env);
}