/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pwd.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:07:59 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:07:59 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	pwd_func(t_shell *shell)
{
	char	pwd[PATH_MAX];

	(void)shell;
	if (!getcwd(pwd, sizeof(pwd)))
		return (perror("getcwd"), 1);
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
