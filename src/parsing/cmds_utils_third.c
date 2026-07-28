/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cmds_utils_third.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:21:39 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:21:39 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	ft_lstadd_back_redirects(t_redir **redir, t_redir *node)
{
	t_redir	*current;

	if (!redir || !node)
		return ;
	if (*redir == NULL)
		*redir = node;
	else
	{
		current = *redir;
		while (current->next != NULL)
			current = current->next;
		current->next = node;
	}
}

int	has_quote(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

int	update_quote_cmd_state(char c, int *single, int *double_quote)
{
	if (c == '\'' && !*double_quote)
	{
		*single = !*single;
		return (1);
	}
	if (c == '"' && !*single)
	{
		*double_quote = !*double_quote;
		return (1);
	}
	return (0);
}

char	*removal_quote_only(char *str)
{
	int		i;
	int		single_quote;
	int		double_quote;
	char	*expanded;

	if (!str)
		return (NULL);
	expanded = ft_strdup("");
	if (!expanded)
		return (NULL);
	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (!update_quote_cmd_state(str[i], &single_quote, &double_quote))
		{
			expanded = append_char(expanded, str[i]);
			if (!expanded)
				return (NULL);
		}
		i++;
	}
	return (expanded);
}

int	set_heredoc_redirect(t_redir *redir, t_token *tokens)
{
	redir->quoted = has_quote(tokens->next->value);
	free(redir->filename);
	redir->filename = removal_quote_only(tokens->next->value);
	if (!redir->filename)
		return (1);
	redir->type = REDIR_HEREDOC;
	return (0);
}
