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

int	finish_cd(char **args, t_shell *shell, char *old_pwd,
		int old_pwd_valid)
{
	char	curr_pwd[PATH_MAX];

	if (!getcwd(curr_pwd, sizeof(curr_pwd)))
	{
		perror("getcwd");
		shell->exit_status = 1;
		return (1);
	}
	if (update_cd_env(shell, old_pwd, curr_pwd, old_pwd_valid))
		return (1);
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_putendl_fd(curr_pwd, STDOUT_FILENO);
	shell->exit_status = 0;
	return (0);
}

int	cd_env_func(char **args, t_shell *shell)
{
	char	old_pwd[PATH_MAX];
	char	*path;
	int		old_pwd_valid;

	if (args[1] && args[2])
		return (cd_error(shell, "cd: too many arguments\n"));
	old_pwd_valid = get_old_pwd(shell, old_pwd);
	path = get_cd_path(args, shell);
	if (!path)
		return (1);
	if (change_directory(path, shell))
		return (1);
	return (finish_cd(args, shell, old_pwd, old_pwd_valid));
}
