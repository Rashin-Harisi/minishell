/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 expansion_utils_third.c							:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:31:14 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:31:14 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

char	*expand_environment_var(char *str, int *i,
	t_shell *shell, char *expanded)
{
	int		start;
	char	*var;
	char	*value;

	start = *i;
	while (str[*i] && is_specific_char(str[*i]))
		(*i)++;
	if (*i == start)
		return (append_char(expanded, '$'));
	var = ft_substr(str, start, *i - start);
	if (!var)
		return (free(expanded), NULL);
	value = find_var_value(var, shell->env);
	free(var);
	if (value)
		return (append_str(expanded, value));
	return (expanded);
}

char	*expand_var(char *str, int *i, t_shell *shell, char *expanded)
{
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (expand_exit_status(shell, expanded));
	}
	return (expand_environment_var(str, i, shell, expanded));
}

int	update_quote_state(char c, int *single_quote,
	int *double_quote)
{
	if (c == '\'' && !*double_quote)
	{
		*single_quote = !*single_quote;
		return (1);
	}
	if (c == '"' && !*single_quote)
	{
		*double_quote = !*double_quote;
		return (1);
	}
	return (0);
}

char	*expand_next_part(char *str, int *i,
	t_shell *shell, char *expanded)
{
	expanded = expand_var(str, i, shell, expanded);
	if (!expanded)
		return (NULL);
	return (expanded);
}
