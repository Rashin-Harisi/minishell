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
	int		count;

	if (!tokens || !*tokens)
		return (NULL);
	count = count_args(*tokens);
	cmd = create_cmd_node(count);
	if (!cmd)
		return (NULL);
	if (fill_cmd(cmd, tokens))
	{
		free_one_cmd(cmd);
		return (NULL);
	}
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
