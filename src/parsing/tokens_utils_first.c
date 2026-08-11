/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 tokens_utils_first.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:41:00 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:41:00 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	update_token_quotes(char c, t_flags *flags)
{
	if (c == '"' && !flags->single_quote)
		flags->double_quote = !flags->double_quote;
	else if (c == '\'' && !flags->double_quote)
		flags->single_quote = !flags->single_quote;
}

int	is_unquoted_operator(char *line, int index, t_flags *flags)
{
	if (flags->single_quote || flags->double_quote)
		return (0);
	if (is_double_operator(line, index))
		return (1);
	if (is_single_operator(line[index]))
		return (1);
	return (0);
}

int	find_part_end(char *line, int *index, t_flags *flags)
{
	while (line[*index])
	{
		update_token_quotes(line[*index], flags);
		if (is_shell_space(line[*index])
			&& !flags->single_quote && !flags->double_quote)
			return (1);
		if (is_unquoted_operator(line, *index, flags))
			return (0);
		(*index)++;
	}
	return (0);
}

char	*extract_word(char *line, int *index, int *syntax_error)
{
	t_flags	flags;
	int		start;
	int		has_space;

	ft_memset(&flags, 0, sizeof(t_flags));
	start = *index;
	has_space = find_part_end(line, index, &flags);
	if (flags.single_quote || flags.double_quote)
	{
		*syntax_error = 1;
		return (NULL);
	}
	if (has_space)
		(*index)++;
	return (ft_substr(line, start, *index - start - has_space));
}

char	*each_part_extract(char *line, int *index, int *syntax_error)
{
	skip_spaces(line, index);
	if (line[*index] == '\0')
		return (NULL);
	if (is_double_operator(line, *index)
		|| is_single_operator(line[*index]))
		return (extract_operator(line, index));
	return (extract_word(line, index, syntax_error));
}
