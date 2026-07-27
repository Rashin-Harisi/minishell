/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cd.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 09:59:20 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 09:59:20 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	cd_env_func(char **args, t_shell *shell)
{
	char	*path;
	char	*oldpwd_env;
	char	*pwd_env;
	char	old_pwd[PATH_MAX];
	char	curr_pwd[PATH_MAX];
	int		old_pwd_valid;

	if (args[1] && args[2])
	{
		ft_putstr_fd("cd : there are too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	old_pwd_valid = 1;
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		old_pwd_valid = 0;
		pwd_env = get_env_value(shell->env, "PWD");
		if (pwd_env)
			ft_strlcpy(old_pwd, pwd_env, sizeof(old_pwd));
	}
	if (!args[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: Home not set\n", 2);
			shell->exit_status = 1;
			return (1);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		oldpwd_env = get_env_value(shell->env, "OLDPWD");
		if (!oldpwd_env)
		{
			ft_putstr_fd("cd: OLDWPD not set\n", 2);
			shell->exit_status = 1;
			return (1);
		}
		path = oldpwd_env;
	}
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		shell->exit_status = 1;
		return (1);
	}
	if (!getcwd(curr_pwd, sizeof(curr_pwd)))
	{
		perror("getcwd");
		shell->exit_status = 1;
		return (1);
	}
	if (old_pwd_valid || old_pwd[0] == '\0')
	{
		if (update_env_value(&shell->env, "OLDPWD", old_pwd))
			return (shell->exit_status = 1, 1);
	}
	if (update_env_value(&shell->env, "PWD", curr_pwd))
		return (shell->exit_status = 1, 1);
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_putendl_fd(curr_pwd, STDOUT_FILENO);
	shell->exit_status = 0;
	return (0);
}
