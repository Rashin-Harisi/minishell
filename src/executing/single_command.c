/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 single_command.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:29:25 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:29:25 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	handle_builtin_redirection(t_shell *shell, t_cmd *cmd,
	int saved[2])
{
	if (!apply_redirection(cmd->redirects))
		return (0);
	restore_standard_fds(saved[0], saved[1]);
	shell->exit_status = 1;
	return (1);
}

int	builtin_with_redirection(t_shell *shell, t_token **tokens)
{
	int	saved[2];
	int	ret;

	if (save_standard_fds(saved))
	{
		shell->exit_status = 1;
		return (1);
	}
	if (handle_builtin_redirection(shell, shell->cmds, saved))
		return (1);
	ret = builtin_functions(shell, tokens);
	if (restore_standard_fds(saved[0], saved[1]))
	{
		shell->exit_status = 1;
		ret = 1;
	}
	return (ret);
}

int	execute_single_command(t_shell *shell, char **paths, t_token **tokens)
{
	t_cmd	*cmds;

	cmds = shell->cmds;
	if (!cmds->args || !cmds->args[0])
		return (only_redirection(cmds, shell));
	if (is_builtin(cmds->args))
	{
		if (cmds->redirects)
			return (builtin_with_redirection(shell, tokens));
		return (builtin_functions(shell, tokens));
	}
	return (execute_external_command(cmds, paths, shell));
}
