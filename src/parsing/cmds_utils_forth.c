/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cmds_utils_forth.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:22:14 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:22:14 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

t_cmd	*create_cmd_node(int count)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->redirects = NULL;
	cmd->args = ft_calloc(count + 1, sizeof(char *));
	cmd->next = NULL;
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

int	is_redirection_token(t_token_type type)
{
	if (type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (1);
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
		return (1);
	return (0);
}

int	add_word_to_cmd(t_cmd *cmd, t_token **tokens, int *index)
{
	if ((*tokens)->value && (*tokens)->value[0] == '\0'
		&& !(*tokens)->quoted)
	{
		*tokens = (*tokens)->next;
		return (0);
	}
	cmd->args[*index] = ft_strdup((*tokens)->value);
	if (!cmd->args[*index])
		return (1);
	(*index)++;
	*tokens = (*tokens)->next;
	return (0);
}

int	add_redirection_to_cmd(t_cmd *cmd, t_token **tokens)
{
	t_redir	*node;

	node = init_redirect(*tokens);
	if (!node)
		return (1);
	ft_lstadd_back_redirects(&cmd->redirects, node);
	*tokens = (*tokens)->next->next;
	return (0);
}

int	fill_cmd(t_cmd *cmd, t_token **tokens)
{
	int	index;

	index = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (add_word_to_cmd(cmd, tokens, &index))
				return (1);
		}
		else if (is_redirection_token((*tokens)->type))
		{
			if (add_redirection_to_cmd(cmd, tokens))
				return (1);
		}
		else
			*tokens = (*tokens)->next;
	}
	cmd->args[index] = NULL;
	return (0);
}
