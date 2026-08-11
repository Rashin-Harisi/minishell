/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 export_utils.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:00:15 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:00:15 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	print_export(t_env *env)
{
	while (env)
	{
		if (env->has_equal)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		env = env->next;
	}
}

static char	*get_export_key(char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (ft_strdup(arg));
	return (ft_substr(arg, 0, equal - arg));
}

static char	*get_export_value(char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (NULL);
	return (ft_strdup(equal + 1));
}

void	free_export_data(t_export *data)
{
	free(data->key);
	free(data->value);
	data->key = NULL;
	data->value = NULL;
}

int	parse_export_arg(t_export *data, char *arg)
{
	data->has_equal = (ft_strchr(arg, '=') != NULL);
	data->key = get_export_key(arg);
	data->value = get_export_value(arg);
	if (!data->key || (data->has_equal && !data->value))
	{
		free_export_data(data);
		return (1);
	}
	return (0);
}
