/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cd_utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:14:03 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:14:03 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	cd_error(t_shell *shell, char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	shell->exit_status = 1;
	return (1);
}

int	get_old_pwd(t_shell *shell, char *old_pwd)
{
	char	*pwd_env;

	old_pwd[0] = '\0';
	if (getcwd(old_pwd, PATH_MAX))
		return (1);
	pwd_env = get_env_value(shell->env, "PWD");
	if (!pwd_env)
		return (0);
	ft_strlcpy(old_pwd, pwd_env, PATH_MAX);
	return (1);
}

char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
			cd_error(shell, "cd: HOME not set\n");
		return (path);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (!path)
			cd_error(shell, "cd: OLDPWD not set\n");
		return (path);
	}
	return (args[1]);
}

int	change_directory(char *path, t_shell *shell)
{
	if (chdir(path) != -1)
		return (0);
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	shell->exit_status = 1;
	return (1);
}

int	update_cd_env(t_shell *shell, char *old_pwd, char *curr_pwd,
		int old_pwd_valid)
{
	if (old_pwd_valid
		&& update_env_value(&shell->env, "OLDPWD", old_pwd))
	{
		shell->exit_status = 1;
		return (1);
	}
	if (update_env_value(&shell->env, "PWD", curr_pwd))
	{
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}
