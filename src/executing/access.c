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

char	*check_path_entry(char *path, char *cmd, t_shell *shell,
		int *found_not_executable)
{
	char	*pathname;

	pathname = create_pathname(path, cmd, shell);
	if (!pathname)
		return (NULL);
	if (access(pathname, F_OK) != 0)
		return (free(pathname), NULL);
	if (is_directory(pathname) || access(pathname, X_OK) != 0)
	{
		*found_not_executable = 1;
		free(pathname);
		return (NULL);
	}
	return (pathname);
}

void	print_command_error(char *cmd, t_shell *shell,
		int found_not_executable)
{
	if (found_not_executable)
	{
		shell->exit_status = 126;
		print_error(cmd, ": Permission denied\n");
	}
	else
	{
		shell->exit_status = 127;
		print_error(cmd, ": command not found\n");
	}
}

char	*search_paths(char **paths, char *cmd, t_shell *shell)
{
	char	*pathname;
	int		found_not_executable;
	int		i;

	i = 0;
	found_not_executable = 0;
	while (paths[i])
	{
		pathname = check_path_entry(paths[i], cmd, shell,
				&found_not_executable);
		if (pathname)
			return (pathname);
		i++;
	}
	print_command_error(cmd, shell, found_not_executable);
	return (NULL);
}

char	*check_access_pathname(char **paths, char *cmd, t_shell *shell)
{
	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/') || !paths)
		return (check_direct_path(cmd, shell));
	return (search_paths(paths, cmd, shell));
}
