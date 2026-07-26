/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 helper.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/26 19:26:23 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/26 19:26:23 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
