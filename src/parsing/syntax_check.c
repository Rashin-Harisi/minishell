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

int	set_redirection_error(t_token *curr, t_shell *shell)
{
	if (curr->next)
		shell->syntax_check = curr->next->value;
	else
		shell->syntax_check = "newline";
	return (1);
}

int	validate_token(t_token *prev, t_token *curr, t_shell *shell)
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
	if (is_redirection(curr->type) && !redir_validation(curr))
		return (set_redirection_error(curr, shell));
	return (0);
}

int	syntax_check(t_token *tokens, t_shell *shell)
{
	t_token	*prev;

	prev = NULL;
	shell->syntax_check = NULL;
	while (tokens)
	{
		if (validate_token(prev, tokens, shell))
			return (1);
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}
