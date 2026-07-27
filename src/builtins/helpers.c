/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 helpers.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 09:54:58 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 09:54:58 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

t_env	*find_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	update_env_value(t_env **env, char *key, char *new_value)
{
	t_env	*node;
	t_env	*new;
	char	*tmp;

	node = find_node(*env, key);
	if (node)
	{
		tmp = ft_strdup(new_value);
		if (!tmp)
			return (1);
		free(node->value);
		node->value = tmp;
		node->has_equal = 1;
		return (0);
	}
	new = create_env_node(key, new_value);
	if (!new)
		return (1);
	new->has_equal = 1;
	ft_lstadd_back_env(env, new);
	return (0);
}

// A word consisting solely of letters, numbers, and underscores,
// and beginning with a letter or underscore.
int	is_valid_name(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (!(str[0] == '_' || ft_isalpha(str[0])))
		return (0);
	while (str[i])
	{
		if (!(str[i] == '_' || ft_isalnum(str[i])))
			return (0);
		i++;
	}
	return (1);
}

void	print_identifier_error(char *func, char *str, char *value)
{
	ft_putstr_fd(func, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(str, 2);
	if (value)
	{
		ft_putstr_fd("=", 2);
		ft_putstr_fd(value, 2);
	}
	ft_putstr_fd("': not a valid identifier\n", 2);
}
