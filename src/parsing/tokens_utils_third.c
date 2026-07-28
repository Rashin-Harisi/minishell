/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 tokens_utils_third.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:42:13 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:42:19 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	token_has_quote(char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

t_token_type	get_token_type(char *value)
{
	if (ft_strncmp(value, ">>", 3) == 0)
		return (TOKEN_APPEND);
	if (ft_strncmp(value, "<<", 3) == 0)
		return (TOKEN_HEREDOC);
	if (ft_strncmp(value, "<", 2) == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strncmp(value, ">", 2) == 0)
		return (TOKEN_REDIR_OUT);
	if (ft_strncmp(value, "|", 2) == 0)
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

t_token	*init_token(char *tmp, t_token *node)
{
	node->value = tmp;
	node->quoted = token_has_quote(tmp);
	node->type = get_token_type(tmp);
	node->next = NULL;
	return (node);
}

void	ft_lstadd_back_token(t_token **token, t_token *new)
{
	t_token	*current;

	if (!token || !new)
		return ;
	if (*token == NULL)
		*token = new;
	else
	{
		current = *token;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
