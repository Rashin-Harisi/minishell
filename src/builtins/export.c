/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 export.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:02:48 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:02:48 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	update_export_node(t_export *data)
{
	char	*new_value;

	if (!data->has_equal)
		return (0);
	new_value = ft_strdup(data->value);
	if (!new_value)
		return (1);
	free(data->node->value);
	data->node->value = new_value;
	data->node->has_equal = 1;
	return (0);
}

int	add_export_node(t_export *data, t_shell *shell)
{
	data->node = create_env_node(data->key, data->value);
	if (!data->node)
		return (1);
	data->node->has_equal = data->has_equal;
	ft_lstadd_back_env(&shell->env, data->node);
	return (0);
}

int	save_export_arg(t_export *data, t_shell *shell)
{
	data->node = find_node(shell->env, data->key);
	if (data->node)
		return (update_export_node(data));
	return (add_export_node(data, shell));
}

int	process_export_arg(char *arg, t_shell *shell, t_export *data)
{
	if (parse_export_arg(data, arg))
		return (1);
	if (!is_valid_name(data->key))
	{
		print_identifier_error("export", data->key, data->value);
		data->status = 1;
		free_export_data(data);
		return (0);
	}
	if (save_export_arg(data, shell))
	{
		free_export_data(data);
		return (1);
	}
	free_export_data(data);
	return (0);
}

int	export_env_func(char **args, t_shell *shell)
{
	t_export	data;
	int			i;

	if (!args[1])
	{
		print_export(shell->env);
		shell->exit_status = 0;
		return (0);
	}
	ft_memset(&data, 0, sizeof(t_export));
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], shell, &data))
		{
			shell->exit_status = 1;
			return (1);
		}
		i++;
	}
	shell->exit_status = data.status;
	return (data.status);
}
