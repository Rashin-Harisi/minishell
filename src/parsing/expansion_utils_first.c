/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 expansion_utils_first.c							:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:30:01 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:30:01 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

char	*append_next_character(char *str, int *i, char *expanded)
{
	expanded = append_char(expanded, str[*i]);
	if (!expanded)
		return (NULL);
	(*i)++;
	return (expanded);
}

char	*expansion_string(char *str, t_shell *shell)
{
	int		i;
	int		single_quote;
	int		double_quote;
	char	*expanded;

	if (!str)
		return (NULL);
	i = 0;
	single_quote = 0;
	double_quote = 0;
	expanded = ft_strdup("");
	if (!expanded)
		return (NULL);
	while (str[i])
	{
		if (update_quote_state(str[i], &single_quote, &double_quote))
			i++;
		else if (str[i] == '$' && !single_quote)
			expanded = expand_next_part(str, &i, shell, expanded);
		else
			expanded = append_next_character(str, &i, expanded);
		if (!expanded)
			return (NULL);
	}
	return (expanded);
}
