/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 tokens_utils_second.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:41:36 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:41:36 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	is_shell_space(char c)
{
	return (c == ' ' || c == '\t');
}

void	skip_spaces(char *line, int *index)
{
	while (line[*index] && is_shell_space(line[*index]))
		(*index)++;
}

int	is_double_operator(char *line, int index)
{
	if (line[index] == '>' && line[index + 1] == '>')
		return (1);
	if (line[index] == '<' && line[index + 1] == '<')
		return (1);
	return (0);
}

int	is_single_operator(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

char	*extract_operator(char *line, int *index)
{
	char	*part;
	int		length;

	length = 1;
	if (is_double_operator(line, *index))
		length = 2;
	part = ft_substr(line, *index, length);
	if (!part)
		return (NULL);
	*index += length;
	return (part);
}
