/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 expansion.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 11:31:56 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 11:31:56 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	expansion_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*tmp;
	t_token	*prev;
	char	*expanded;

	tmp = tokens;
	prev = NULL;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && !(prev && prev->type == TOKEN_HEREDOC))
		{
			expanded = expansion_string(tmp->value, shell);
			if (!expanded)
				return (1);
			free(tmp->value);
			tmp->value = expanded;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (0);
}

int	expansion_args(char **args, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args && args[i])
	{
		expanded = expansion_string(args[i], shell);
		if (!expanded)
			return (1);
		free(args[i]);
		args[i] = expanded;
		i++;
	}
	return (0);
}

int	expansion_redir(t_redir *redir, t_shell *shell)
{
	char	*expanded;

	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			expanded = expansion_string(redir->filename, shell);
			if (!expanded)
				return (1);
			free(redir->filename);
			redir->filename = expanded;
		}
		redir = redir->next;
	}
	return (0);
}

int	expansion(t_shell *shell)
{
	t_cmd	*cmds;

	if (!shell)
		return (1);
	cmds = shell->cmds;
	while (cmds)
	{
		if (expansion_redir(cmds->redirects, shell))
			return (1);
		cmds = cmds->next;
	}
	return (0);
}
