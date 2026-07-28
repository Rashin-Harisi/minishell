/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_utils_first.c.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabdolho <rabdolho@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 14:20:39 by rabdolho          #+#    #+#             */
/*   Updated: 2026/07/28 14:21:03 by rabdolho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	set_redirect_type(t_redir *redir, t_token *tokens)
{
	if (tokens->type == TOKEN_APPEND)
		redir->type = REDIR_APPEND;
	else if (tokens->type == TOKEN_HEREDOC)
		return (set_heredoc_redirect(redir, tokens));
	else if (tokens->type == TOKEN_REDIR_IN)
		redir->type = REDIR_IN;
	else if (tokens->type == TOKEN_REDIR_OUT)
		redir->type = REDIR_OUT;
	else
		return (1);
	return (0);
}

t_redir	*init_redirect(t_token *tokens)
{
	t_redir	*redir;

	if (!tokens || !tokens->next || !tokens->next->value)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->fd = -1;
	redir->quoted = 0;
	redir->next = NULL;
	redir->filename = ft_strdup(tokens->next->value);
	if (!redir->filename)
		return (free(redir), NULL);
	if (set_redirect_type(redir, tokens))
	{
		free(redir->filename);
		free(redir);
		return (NULL);
	}
	return (redir);
}

int	is_redirection_type(t_token_type type)
{
	if (type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (1);
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
		return (1);
	return (0);
}

int	is_valid_word(t_token *token)
{
	if (token->type != TOKEN_WORD)
		return (0);
	if (!token->value)
		return (0);
	if (token->value[0] != '\0' || token->quoted)
		return (1);
	return (0);
}

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redirection_type(tokens->type) && tokens->next)
		{
			tokens = tokens->next->next;
			continue ;
		}
		if (is_valid_word(tokens))
			count++;
		tokens = tokens->next;
	}
	return (count);
}
