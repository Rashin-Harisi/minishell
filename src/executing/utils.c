/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:44:54 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:44:54 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	count_node(t_cmd *cmds)
{
	int		i;
	t_cmd	*tmp;

	tmp = cmds;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	is_builtin(char **args)
{
	if (!args || !args[0])
		return (0);
	if (ft_strncmp(args[0], "cd", 3) == 0
		|| ft_strncmp(args[0], "exit", 5) == 0
		|| ft_strncmp(args[0], "env", 4) == 0
		|| ft_strncmp(args[0], "export", 7) == 0
		|| ft_strncmp(args[0], "unset", 6) == 0
		|| ft_strncmp(args[0], "echo", 5) == 0
		|| ft_strncmp(args[0], "pwd", 4) == 0)
		return (1);
	return (0);
}

int	calculate_nodes(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		if (env->has_equal)
			i++;
		env = env->next;
	}
	return (i);
}

char	*create_env_line(t_env *env)
{
	char	*key_equal;
	char	*line;

	key_equal = ft_strjoin(env->key, "=");
	if (!key_equal)
		return (NULL);
	line = ft_strjoin(key_equal, env->value);
	free(key_equal);
	return (line);
}

char	**convert_list_to_array(t_env *env)
{
	char	**envp;
	int		i;

	envp = ft_calloc(calculate_nodes(env) + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->has_equal)
		{
			envp[i] = create_env_line(env);
			if (!envp[i])
				return (free_array(envp), NULL);
			i++;
		}
		env = env->next;
	}
	return (envp);
}
