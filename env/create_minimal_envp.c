/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 create_minimal_envp.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/26 19:13:11 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/26 19:13:11 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	node->has_equal = 1;
	if (!node->key || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

static int	add_node_to_env(t_env **env, char *key, char *value)
{
	t_env	*node;

	node = create_env_node(key, value);
	if (!node)
		return (0);
	ft_lstadd_back_env(env, node);
	return (1);
}

t_env	*create_minimal_envp(void)
{
	t_env	*env;
	char	cwd[PATH_MAX];

	env = NULL;
	if (!add_node_to_env(&env, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin"))
		return (free_envs(env), NULL);
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd() error");
		free_envs(env);
		return (NULL);
	}
	if (!add_node_to_env(&env, "PWD", cwd))
		return (free_envs(env), NULL);
	if (!add_node_to_env(&env, "SHLVL", "1"))
		return (free_envs(env), NULL);
	return (env);
}
