/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 update_env.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:09:55 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:09:55 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	builtin_update_env(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strncmp(cmd->args[0], "cd", ft_strlen("cd") + 1) == 0)
	{
		if (cd_env_func(cmd->args, shell))
			return (1);
	}
	if (ft_strncmp(cmd->args[0], "export", ft_strlen("export") + 1) == 0)
	{
		if (export_env_func(cmd->args, shell))
			return (1);
	}
	if (ft_strncmp(cmd->args[0], "unset", ft_strlen("unset") + 1) == 0)
	{
		if (unset_env_func(cmd->args, shell))
			return (1);
	}
	if (ft_strncmp(cmd->args[0], "env", ft_strlen("env") + 1) == 0)
	{
		if (env_func(shell))
			return (1);
	}
	return (0);
}
