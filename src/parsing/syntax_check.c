/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 syntax_check.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 11:38:52 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 11:38:52 by rabdolho		  ###	########.fr		  */
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

int	syntax_check(t_token *tokens, t_shell *shell)
{
	t_token	*prev;
	t_token	*curr;

	prev = NULL;
	curr = tokens;
	shell->syntax_check = NULL;
	while (curr)
	{
		if (has_forbidden_chart(curr))
		{
			shell->syntax_check = curr->value;
			return (1);
		}
		if (curr->type == TOKEN_PIPE && !pipe_validation(prev, curr))
		{
			shell->syntax_check = curr->value;
			return (1);
		}
		if ((curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC
				|| curr->type == TOKEN_REDIR_IN
				|| curr->type == TOKEN_REDIR_OUT)
			&& !redir_validation(curr))
		{
			if (curr->next)
				shell->syntax_check = curr->next->value;
			else
				shell->syntax_check = "newline";
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
