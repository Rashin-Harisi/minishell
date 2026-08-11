/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:25:11 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:25:11 by rabdolho		  ###	########.fr		  */
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

int	heredoc_error(int fd, char *filename)
{
	if (fd != -1)
		close(fd);
	if (filename)
	{
		unlink(filename);
		free(filename);
	}
	return (1);
}

char	*expand_heredoc_line(char *line, t_redir *redir,
		t_shell *shell)
{
	char	*expanded;

	if (redir->quoted)
		return (line);
	expanded = expansion_string(line, shell);
	free(line);
	return (expanded);
}

int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == -1)
		return (1);
	if (write(fd, "\n", 1) == -1)
		return (1);
	return (0);
}
