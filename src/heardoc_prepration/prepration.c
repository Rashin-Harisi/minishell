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

char	*create_name(int index)
{
	char	*number;
	char	*filename;

	number = ft_itoa(index);
	if (!number)
		return (NULL);
	filename = ft_strjoin(".heredoc_", number);
	free(number);
	if (!filename)
		return (NULL);
	return (filename);
}

int	prepare_reading_file(t_redir *redir, t_shell *shell)
{
	char	*line;
	int		fd;
	char	*expanded;
	char	*filename;

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
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strncmp(line, redir->filename,
				ft_strlen(redir->filename) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (!redir->quoted)
		{
			expanded = expansion_string(line, shell);
			free(line);
			line = expanded;
			if (!line)
			{
				close(fd);
				unlink(filename);
				free(filename);
				return (1);
			}
		}
		if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
		{
			perror("heredoc");
			free(line);
			close(fd);
			unlink(filename);
			free(filename);
			return (1);
		}
		free(line);
	}
	if (close(fd) == -1)
	{
		perror("heredoc");
		unlink(filename);
		free(filename);
		return (1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("heredoc");
		unlink(filename);
		free(filename);
		return (1);
	}
	unlink(filename);
	free(filename);
	if (redir->fd != -1)
		close(redir->fd);
	redir->fd = fd;
	return (0);
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
