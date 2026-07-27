/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 access.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:12:07 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:12:07 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

void	print_error(char *s1, char *s2)
{
	char	*msg;

	msg = ft_strjoin(s1, s2);
	if (!msg)
		return ;
	write(STDERR_FILENO, msg, ft_strlen(msg));
	free(msg);
}

char	*check_access_pathname(char **paths, char *cmd, t_shell *shell)
{
	int		i;
	char	*temp;
	char	*pathname;
	int		fount_not_executable;

	i = 0;
	fount_not_executable = 0;
	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (is_directory(cmd))
		{
			shell->exit_status = 126;
			print_error(cmd, ": Is a directory\n");
			return (NULL);
		}
		if (access(cmd, F_OK) != 0)
		{
			perror(cmd);
			shell->exit_status = 127;
			return (NULL);
		}
		if (access(cmd, X_OK) != 0)
		{
			perror(cmd);
			shell->exit_status = 126;
			return (NULL);
		}
		pathname = ft_strdup(cmd);
		if (!pathname)
			shell->exit_status = 1;
		return (pathname);
	}
	if (!paths)
	{
		shell->exit_status = 127;
		print_error(cmd, ": command not found\n");
		return (NULL);
	}
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (shell->exit_status = 1, NULL);
		pathname = ft_strjoin(temp, cmd);
		free(temp);
		if (!pathname)
			return (shell->exit_status = 1, NULL);
		if (access(pathname, F_OK) == 0)
		{
			if (is_directory(pathname))
			{
				fount_not_executable = 1;
				free(pathname);
				i++;
				continue ;
			}
			if (access(pathname, X_OK) == 0)
				return (pathname);
			fount_not_executable = 1;
		}
		free(pathname);
		i++;
	}
	if (fount_not_executable)
	{
		shell->exit_status = 126;
		print_error(cmd, ": Permission denied\n");
	}
	else
	{
		shell->exit_status = 127;
		print_error(cmd, ": command not found\n");
	}
	return (NULL);
}
