/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cmds_utils_second.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:21:14 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:21:14 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_redirects(t_redir *redir)
{
	t_redir	*cur;

	while (redir)
	{
		cur = redir->next;
		if (redir->type == REDIR_HEREDOC && redir->fd >= 0)
			close(redir->fd);
		free(redir->filename);
		free(redir);
		redir = cur;
	}
}

void	free_one_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_array(cmd->args);
	free_redirects(cmd->redirects);
	free(cmd);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*cur;

	while (cmds)
	{
		cur = cmds->next;
		free_one_cmd(cmds);
		cmds = cur;
	}
}

void	ft_lstadd_back_cmds(t_cmd **cmds, t_cmd *node)
{
	t_cmd	*current;

	if (!cmds || !node)
		return ;
	if (*cmds == NULL)
		*cmds = node;
	else
	{
		current = *cmds;
		while (current->next != NULL)
			current = current->next;
		current->next = node;
	}
}
