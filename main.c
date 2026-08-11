/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 main.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/26 17:20:39 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/26 17:21:38 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	handle_line(t_main *data)
{
	if (is_empty_line(data->shell.line))
	{
		free(data->shell.line);
		return (1);
	}
	if (data->shell.interactive)
		add_history(data->shell.line);
	if (prepare_commands(data))
	{
		free_iteration(data->tokens, data->shell.cmds, data->shell.line);
		return (1);
	}
	return (0);
}

int	execute_commands(t_main *data)
{
	int	ret;

	free_paths(data->paths);
	data->paths = get_paths(data->shell.env);
	ret = execution(&data->shell, &data->tokens, data->paths);
	free_iteration(data->tokens, data->shell.cmds, data->shell.line);
	if (ret == 2)
		return (2);
	return (1);
}

int	shell_iteration(t_main *data)
{
	data->tokens = NULL;
	data->shell.cmds = NULL;
	set_prompt_signals();
	data->shell.line = read_shell_line(&data->shell);
	if (!data->shell.line)
	{
		if (data->shell.interactive)
			ft_putstr_fd("exit\n", STDERR_FILENO);
		return (2);
	}
	if (handle_line(data))
		return (1);
	return (execute_commands(data));
}

void	cleanup_shell(t_main *data)
{
	if (data->paths)
		free_paths(data->paths);
	free_envs(data->shell.env);
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_main	data;
	int		ret;

	(void)argv;
	if (argc != 1)
		return (1);
	init_main(&data, envp);
	while (1)
	{
		ret = shell_iteration(&data);
		if (ret == 2)
			break ;
	}
	cleanup_shell(&data);
	return (data.shell.exit_status);
}
