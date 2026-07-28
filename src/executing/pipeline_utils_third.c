/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pipeline_utils_third.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:51:24 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:51:24 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	redirect_child_output(t_pipeline *data)
{
	if (!data->cmd->next)
		return (0);
	if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close_current_pipe(data);
		return (1);
	}
	close_current_pipe(data);
	return (0);
}

int	prepare_child_fds(t_pipeline *data)
{
	if (redirect_child_input(data))
	{
		if (data->cmd->next)
			close_current_pipe(data);
		return (1);
	}
	if (redirect_child_output(data))
		return (1);
	return (0);
}

void	exit_pipeline_child(t_shell *shell, t_pipeline *data,
	t_token **tokens, int status)
{
	free_child_pipeline(shell, data, tokens);
	exit(status);
}

void	execute_child_builtin(t_shell *shell, t_pipeline *data,
	t_token **tokens)
{
	int	builtin_status;

	shell->in_pipe = 1;
	builtin_status = builtin_functions_in_pipe(shell, tokens, data->cmd);
	if (builtin_status != 2)
		shell->exit_status = builtin_status;
	exit_pipeline_child(shell, data, tokens, shell->exit_status);
}

void	execute_pipeline_child(t_shell *shell, t_pipeline *data,
	t_token **tokens)
{
	set_child_signals();
	if (prepare_child_fds(data))
		exit_pipeline_child(shell, data, tokens, 1);
	if (apply_redirection(data->cmd->redirects))
		exit_pipeline_child(shell, data, tokens, 1);
	if (is_builtin(data->cmd->args))
		execute_child_builtin(shell, data, tokens);
	if (external_command_in_pipe(data->paths, data->cmd,
			shell, data->envp))
		exit_pipeline_child(shell, data, tokens, shell->exit_status);
}
