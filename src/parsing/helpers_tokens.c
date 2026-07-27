/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 helpers_tokens.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 11:21:28 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 11:21:28 by rabdolho		  ###	########.fr		  */
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

char	*each_part_extract(char *line, int *index, int *syntax_error)
{
	t_flags	flags;
	int		start;
	int		len;
	char	*tmp;

	tmp = NULL;
	len = 0;
	ft_memset(&flags, 0, sizeof(t_flags));
	skip_spaces(line, index);
	if (line[*index] == '\0')
		return (NULL);
	if ((line[*index] == '>' && line[*index + 1] == '>')
		|| (line[*index] == '<' && line[*index + 1] == '<' ))
	{
		tmp = malloc(3 * sizeof(char));
		if (!tmp)
			return (NULL);
		tmp[0] = line[*index];
		tmp[1] = line[*index + 1];
		tmp[2] = '\0';
		(*index) += 2;
		return (tmp);
	}
	if ((line[*index] == '>')
		|| (line[*index] == '<')
		|| (line[*index] == '|'))
	{
		tmp = malloc(2 * sizeof(char));
		if (!tmp)
			return (NULL);
		tmp[0] = line[*index];
		tmp[1] = '\0';
		(*index) += 1;
		return (tmp);
	}
	start = *index;
	while (line[*index] != '\0')
	{
		if (line[*index] == '"' && !flags.single_quote)
			flags.double_quote = !flags.double_quote;
		if (line[*index] == '\'' && !flags.double_quote)
			flags.single_quote = !flags.single_quote;
		if (is_shell_space(line[*index])
			&& (!flags.single_quote && !flags.double_quote))
		{
			len = (*index) - start;
			(*index)++;
			break ;
		}
		if ((line[*index] == '>' && line[*index + 1] == '>'
				&& !flags.single_quote && !flags.double_quote)
			|| (line[*index] == '<' && line[*index + 1] == '<'
				&& !flags.single_quote && !flags.double_quote)
			|| (line[*index] == '>' && !flags.single_quote
				&& !flags.double_quote)
			|| (line[*index] == '<'
				&& !flags.single_quote && !flags.double_quote)
			|| (line[*index] == '|'
				&& !flags.single_quote && !flags.double_quote))
		{
			len = (*index) - start;
			break ;
		}
		(*index)++;
	}
	if (flags.double_quote || flags.single_quote)
	{
		(*syntax_error) = 1;
		return (NULL);
	}
	if (len == 0)
		len = (*index) - start;
	tmp = ft_substr(line, start, len);
	return (tmp);
}
