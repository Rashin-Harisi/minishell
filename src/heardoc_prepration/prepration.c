/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 prepration.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:48:43 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:48:43 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	read_heredoc(int fd, t_redir *redir, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			return (0);
		if (!ft_strncmp(line, redir->filename,
				ft_strlen(redir->filename) + 1))
			return (free(line), 0);
		line = expand_heredoc_line(line, redir, shell);
		if (!line)
			return (1);
		if (write_heredoc_line(fd, line))
			return (perror("heredoc"), free(line), 1);
		free(line);
	}
}

int	open_heredoc_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		perror("heredoc");
	return (fd);
}

int	finish_heredoc(t_redir *redir, int fd, char *filename)
{
	if (close(fd) == -1)
	{
		perror("heredoc");
		return (heredoc_error(-1, filename));
	}
	fd = open_heredoc_file(filename);
	if (fd == -1)
		return (heredoc_error(-1, filename));
	unlink(filename);
	free(filename);
	if (redir->fd != -1)
		close(redir->fd);
	redir->fd = fd;
	return (0);
}

int	prepare_reading_file(t_redir *redir, t_shell *shell)
{
	char	*filename;
	int		fd;

	filename = create_name(shell->heredoc_index++);
	if (!filename)
		return (1);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc");
		free(filename);
		return (1);
	}
	if (read_heredoc(fd, redir, shell))
		return (heredoc_error(fd, filename));
	return (finish_heredoc(redir, fd, filename));
}

int	heredoc_preparation(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*tmp;
	t_redir	*redir;

	tmp = cmds;
	while (tmp)
	{
		redir = tmp->redirects;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (prepare_reading_file(redir, shell))
					return (1);
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
	return (0);
}
