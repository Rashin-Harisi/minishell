/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/26 20:00:56 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/26 20:00:56 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("TOKEN_WORD");
	if (type == TOKEN_PIPE)
		return ("TOKEN_PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("TOKEN_REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("TOKEN_REDIR_OUT");
	if (type == TOKEN_APPEND)
		return ("TOKEN_APPEND");
	if (type == TOKEN_HEREDOC)
		return ("TOKEN_HEREDOC");
	return ("UNKNOWN");
}

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
