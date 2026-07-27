/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cmds.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 11:08:11 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 11:08:11 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

t_cmd	*init_cmds(t_token **tokens)
{
	t_cmd	*cmd;
	int		i;
	t_redir	*node;
	int		count;

	if (!tokens || !*tokens)
		return (NULL);
	i = 0;
	count = count_args(*tokens);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->redirects = NULL;
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->args = ft_calloc((count + 1), sizeof(char *));
	if (!cmd->args)
		return (free(cmd), NULL);
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if ((*tokens)->value && (*tokens)->value[0] == '\0'
				&& !(*tokens)->quoted)
			{
				(*tokens) = (*tokens)->next;
				continue ;
			}
			cmd->args[i] = ft_strdup((*tokens)->value);
			if (!cmd->args[i])
				return (free_one_cmd(cmd), NULL);
			i++;
			(*tokens) = (*tokens)->next;
			continue ;
		}
		if ((*tokens)->type == TOKEN_APPEND
			|| (*tokens)->type == TOKEN_HEREDOC
			|| (*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT)
		{
			node = init_redirect(*tokens);
			if (!node)
				return (free_one_cmd(cmd), NULL);
			ft_lstadd_back_redirects(&cmd->redirects, node);
			(*tokens) = (*tokens)->next->next;
			continue ;
		}
		(*tokens) = (*tokens)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}

t_cmd	*create_cmds(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*tmp;

	cmds = NULL;
	while (tokens)
	{
		tmp = init_cmds(&tokens);
		if (!tmp)
			return (free_cmds(cmds), NULL);
		ft_lstadd_back_cmds(&cmds, tmp);
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (cmds);
}
