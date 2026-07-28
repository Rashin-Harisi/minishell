/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pipeline_utils_forth.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:51:53 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:51:53 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	update_parent_fds(t_pipeline *data)
{
	close_previous_fd(data);
	if (data->cmd->next)
	{
		close(data->pipefd[1]);
		data->prev_fd = data->pipefd[0];
	}
}

int	create_child(t_shell *shell, t_pipeline *data,
	t_token **tokens)
{
	data->pids[data->index] = fork();
	if (data->pids[data->index] == -1)
	{
		perror("fork");
		return (pipeline_error(shell, data, data->cmd->next != NULL));
	}
	if (data->pids[data->index] == 0)
		execute_pipeline_child(shell, data, tokens);
	update_parent_fds(data);
	return (0);
}

int	run_pipeline(t_shell *shell, t_pipeline *data,
	t_token **tokens)
{
	while (data->cmd)
	{
		if (create_pipe(shell, data))
			return (1);
		if (create_child(shell, data, tokens))
			return (1);
		data->cmd = data->cmd->next;
		data->index++;
	}
	return (0);
}

void	wait_pipeline(t_shell *shell, t_pipeline *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		if (wait_for_pid(data->pids[i], &data->status)
			&& i == data->count - 1)
			shell->exit_status = 1;
		if (i == data->count - 1)
			set_wait_status(shell, data->status);
		i++;
	}
}

int	init_pipeline(t_shell *shell, char **paths, t_pipeline *data)
{
	ft_memset(data, 0, sizeof(t_pipeline));
	data->cmd = shell->cmds;
	data->paths = paths;
	data->prev_fd = -1;
	data->envp = convert_list_to_array(shell->env);
	if (!data->envp)
		return (1);
	data->count = count_node(data->cmd);
	data->pids = malloc(data->count * sizeof(pid_t));
	if (!data->pids)
	{
		free_array(data->envp);
		return (1);
	}
	return (0);
}
