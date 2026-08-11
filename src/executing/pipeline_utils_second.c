/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pipeline_utils_second.c							:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:50:53 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:50:53 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	close_current_pipe(t_pipeline *data)
{
	close(data->pipefd[0]);
	close(data->pipefd[1]);
}

void	stop_pipeline_children(t_pipeline *data)
{
	int	i;

	i = 0;
	while (i < data->index)
	{
		kill(data->pids[i], SIGTERM);
		i++;
	}
	i = 0;
	while (i < data->index)
	{
		wait_for_pid(data->pids[i], &data->status);
		i++;
	}
}

int	pipeline_error(t_shell *shell, t_pipeline *data,
	int pipe_created)
{
	close_previous_fd(data);
	if (pipe_created)
		close_current_pipe(data);
	stop_pipeline_children(data);
	free_array(data->envp);
	free(data->pids);
	shell->exit_status = 1;
	set_prompt_signals();
	return (1);
}

int	create_pipe(t_shell *shell, t_pipeline *data)
{
	if (!data->cmd->next)
		return (0);
	if (pipe(data->pipefd) == 0)
		return (0);
	perror("pipe");
	return (pipeline_error(shell, data, 0));
}

int	redirect_child_input(t_pipeline *data)
{
	if (data->prev_fd == -1)
		return (0);
	if (dup2(data->prev_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close_previous_fd(data);
		return (1);
	}
	close_previous_fd(data);
	return (0);
}
