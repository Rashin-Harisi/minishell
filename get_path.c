#include "minishell.h"

char	**get_paths(t_env *env)
{
	while (env)
    {
        if(ft_strncmp(env->key, "PATH", 5) == 0)
        {
            if (!env->value)
                return (NULL);
			return (ft_split(env->value, ':'));
        }
		env = env->next;
	}
	return (NULL);
}