/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 syntax_utils.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:46:35 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:46:35 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	pipe_validation(t_token *prev, t_token *curr)
{
	t_token	*next;

	if (!curr)
		return (0);
	next = curr->next;
	if (next == NULL || prev == NULL)
		return (0);
	if (prev->type == TOKEN_PIPE || next->type == TOKEN_PIPE)
		return (0);
	if (prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC
		|| prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT)
		return (0);
	return (1);
}

int	redir_validation(t_token *curr)
{
	t_token	*next;

	if (!curr)
		return (0);
	next = curr->next;
	if (next == NULL)
		return (0);
	if (next->type != TOKEN_WORD)
		return (0);
	return (1);
}

int	has_forbidden_chart(t_token *curr)
{
	int	len;

	if (!curr || !curr->value)
		return (1);
	len = ft_strlen(curr->value);
	if (ft_strncmp(curr->value, ";", 2) == 0
		|| curr->value[len -1] == ';')
		return (1);
	if (ft_strncmp(curr->value, "&", 2) == 0
		|| curr->value[len -1] == '&')
		return (1);
	if (ft_strncmp(curr->value, "`", 2) == 0
		|| curr->value[len -1] == '`')
		return (1);
	if ((curr->value[0] == '(' && curr->value[len - 1] == ')')
		|| (curr->value[0] == '(' || curr->value[len - 1] == ')'))
		return (1);
	return (0);
}

int	is_redirection(t_token_type type)
{
	if (type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (1);
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
		return (1);
	return (0);
}
