/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pipeline_command.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 10:15:09 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 10:15:09 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	close_previous_fd(t_pipeline *data)
{
	if (data->prev_fd != -1)
	{
		close(data->prev_fd);
		data->prev_fd = -1;
	}
}

int	execute_pipeline(t_shell *shell, char **paths, t_token **tokens)
{
	t_pipeline	data;

	if (init_pipeline(shell, paths, &data))
	{
		shell->exit_status = 1;
		return (1);
	}
	set_parent_wait_signals();
	if (run_pipeline(shell, &data, tokens))
		return (1);
	wait_pipeline(shell, &data);
	set_prompt_signals();
	free_array(data.envp);
	free(data.pids);
	return (0);
}
