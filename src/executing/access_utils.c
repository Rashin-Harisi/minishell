/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 access_utils.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:31:58 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:31:58 by rabdolho		  ###	########.fr		  */
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

char	*check_direct_path(char *cmd, t_shell *shell)
{
	char	*pathname;

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

char	*create_pathname(char *path, char *cmd, t_shell *shell)
{
	char	*temp;
	char	*pathname;

	temp = ft_strjoin(path, "/");
	if (!temp)
	{
		shell->exit_status = 1;
		return (NULL);
	}
	pathname = ft_strjoin(temp, cmd);
	free(temp);
	if (!pathname)
		shell->exit_status = 1;
	return (pathname);
}
