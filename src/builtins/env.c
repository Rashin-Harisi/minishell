/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 env.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:01:41 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:01:41 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	env_func(t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env;
	while (tmp)
	{
		if (tmp->has_equal)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	shell->exit_status = 0;
	return (0);
}
