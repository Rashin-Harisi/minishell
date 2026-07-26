/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 get_path.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/26 19:23:35 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/26 19:23:35 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	**get_paths(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->key, "PATH", 5) == 0)
		{
			if (!env->value || !env->has_equal)
				return (NULL);
			return (ft_split(env->value, ':'));
		}
		env = env->next;
	}
	return (NULL);
}
