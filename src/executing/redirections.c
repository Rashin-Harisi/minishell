/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 redirections.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:42:13 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:42:13 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	only_redirection(t_cmd *cmd, t_shell *shell)
{
	int	saved[2];
	int	status;
	int	restore_status;

	if (save_standard_fds(saved))
	{
		shell->exit_status = 1;
		return (1);
	}
	status = apply_redirection(cmd->redirects);
	restore_status = restore_standard_fds(saved);
	if (status || restore_status)
	{
		shell->exit_status = 1;
		return (1);
	}
	shell->exit_status = 0;
	return (0);
}

int	get_redirection_fd(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_HEREDOC)
	{
		fd = redir->fd;
		redir->fd = -1;
		return (fd);
	}
	return (open_redirection_file(redir));
}

int	duplicate_redirection_fd(t_redir *redir, int fd)
{
	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup2"), 1);
	}
	else if (redir->type == REDIR_OUT
		|| redir->type == REDIR_APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (perror("dup2"), 1);
	}
	return (0);
}

int	apply_single_redirection(t_redir *redir)
{
	int	fd;

	fd = get_redirection_fd(redir);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	if (duplicate_redirection_fd(redir, fd))
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirection(t_redir *redirects)
{
	while (redirects)
	{
		if (apply_single_redirection(redirects))
			return (1);
		redirects = redirects->next;
	}
	return (0);
}
