/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pipeline_utils_first.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 13:50:15 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 13:50:15 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	external_command_in_pipe(char **paths, t_cmd *cmd,
	t_shell *shell, char **envp)
{
	char	*pathname;

	pathname = check_access_pathname(paths, cmd->args[0], shell);
	if (!pathname)
		return (1);
	execve(pathname, cmd->args, envp);
	perror(cmd->args[0]);
	free(pathname);
	shell->exit_status = 126;
	return (1);
}

int	pipe_pwd(t_shell *shell)
{
	if (pwd_func(shell))
	{
		ft_putstr_fd("minishell : pwd: error\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	pipe_echo(t_shell *shell, t_cmd *cmd)
{
	if (echo_func(shell, cmd))
	{
		ft_putstr_fd("minishell : echo: error\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	builtin_functions_in_pipe(t_shell *shell, t_token **tokens, t_cmd *cmd)
{
	if (!shell || !cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (builtin_update_env(shell, cmd))
		return (1);
	if (!ft_strncmp(cmd->args[0], "pwd", 4))
		return (pipe_pwd(shell));
	if (!ft_strncmp(cmd->args[0], "echo", 5))
		return (pipe_echo(shell, cmd));
	if (!ft_strncmp(cmd->args[0], "exit", 5))
		return (exit_func(shell, *tokens, cmd));
	return (0);
}

void	free_child_pipeline(t_shell *shell, t_pipeline *data,
	t_token **tokens)
{
	if (tokens && *tokens)
		free_tokens(*tokens);
	free_cmds(shell->cmds);
	free_envs(shell->env);
	free(shell->line);
	free_paths(data->paths);
	free_array(data->envp);
	free(data->pids);
}
