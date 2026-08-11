/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 unset.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 09:57:10 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 09:57:10 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	remove_env_node(t_env **env, t_env *node)
{
	t_env	*curr;
	t_env	*prev;

	if (!env || !*env || !node)
		return (1);
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
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int	unset_env_func(char **args, t_shell *shell)
{
	int		i;
	t_env	*node;

	i = 1;
	while (args[i])
	{
		if (is_valid_name(args[i]))
		{
			node = find_node(shell->env, args[i]);
			if (node)
			{
				if (remove_env_node(&shell->env, node))
				{
					shell->exit_status = 1;
					return (1);
				}
			}
		}
		i++;
	}
	shell->exit_status = 0;
	return (0);
}
