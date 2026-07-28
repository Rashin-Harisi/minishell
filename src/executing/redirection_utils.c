/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 redirection_utils.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:58:17 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:58:17 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	open_redirection_file(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (open(redir->filename, O_RDONLY));
	if (redir->type == REDIR_OUT)
		return (open(redir->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == REDIR_APPEND)
		return (open(redir->filename,
				O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

int	validate_redirection(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_HEREDOC)
	{
		if (redir->fd == -1)
			return (1);
		return (0);
	}
	if (redir->type != REDIR_IN && redir->type != REDIR_OUT
		&& redir->type != REDIR_APPEND)
		return (0);
	fd = open_redirection_file(redir);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	close(fd);
	return (0);
}

int	check_redirection(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirects;
		while (redir)
		{
			if (validate_redirection(redir))
				return (1);
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	return (0);
}

int	restore_standard_fds(int saved[2])
{
	int	status;

	status = 0;
	if (dup2(saved[0], STDIN_FILENO) == -1)
		status = 1;
	if (dup2(saved[1], STDOUT_FILENO) == -1)
		status = 1;
	close(saved[0]);
	close(saved[1]);
	if (status)
		perror("dup2");
	return (status);
}
