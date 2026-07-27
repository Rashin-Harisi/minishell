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

int	external_command_in_pipe(char **paths, t_cmd *cmd,
	t_shell *shell, char **envp)
{
	char	*pathname;

	pathname = NULL;
	pathname = check_access_pathname(paths, cmd->args[0], shell);
	if (!pathname)
		return (1);
	if (execve(pathname, cmd->args, envp) == -1)
	{
		perror(cmd->args[0]);
		free(shell->line);
		free_array(envp);
		free_paths(paths);
		free(pathname);
		free_cmds(shell->cmds);
		free_envs(shell->env);
		exit(126);
	}
	return (0);
}

int	builtin_functions_in_pipe(t_shell *shell, t_token **tokens, t_cmd *cmd)
{
	if (!shell || !cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (builtin_update_env(shell, cmd))
		return (1);
	if (ft_strncmp(cmd->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
	{
		if (pwd_func(shell))
		{
			ft_putstr_fd("minishell : pwd: error\n", STDERR_FILENO);
			return (1);
		}
	}
	if (ft_strncmp(cmd->args[0], "echo", ft_strlen("echo") + 1) == 0)
	{
		if (echo_func(shell, cmd))
		{
			ft_putstr_fd("minishell : echo: error\n", STDERR_FILENO);
			return (1);
		}
	}
	if (ft_strncmp(cmd->args[0], "exit", ft_strlen("exit") + 1) == 0)
	{
		return (exit_func(shell, *tokens, cmd));
	}
	return (0);
}

void	free_child_pipeline(t_shell *shell, char **paths,
	char **envp, pid_t *pids, t_token **tokens)
{
	if (tokens && *tokens)
		free_tokens(*tokens);
	free_cmds(shell->cmds);
	free_envs(shell->env);
	free(shell->line);
	free_paths(paths);
	free_array(envp);
	free(pids);
}

int	execute_pipeline(t_shell *shell, char **paths, t_token **tokens)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	*pids;
	int		status;
	char	**envp;
	t_cmd	*cmds;
	int		count;
	int		i;
	int		j;
	int		builtin_status;

	cmds = shell->cmds;
	envp = convert_list_to_array(shell->env);
	if (!envp)
		return (1);
	count = count_node(cmds);
	pids = malloc((count) * sizeof(pid_t));
	if (!pids)
		return (free_array(envp), 1);
	prev_fd = -1;
	i = 0;
	set_parent_wait_signals();
	while (cmds)
	{
		if (cmds->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			if (prev_fd != -1)
				close(prev_fd);
			j = 0;
			while (j < i)
			{
				kill(pids[j], SIGTERM);
				j++;
			}
			j = 0;
			while (j < i)
			{
				wait_for_pid(pids[j], &status);
				j++;
			}
			free_array(envp);
			free(pids);
			shell->exit_status = 1;
			set_prompt_signals();
			return (1);
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			if (prev_fd != -1)
				close(prev_fd);
			if (cmds->next)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			j = 0;
			while (j < i)
			{
				kill(pids[j], SIGTERM);
				j++;
			}
			j = 0;
			while (j < i)
			{
				wait_for_pid(pids[j], &status);
				j++;
			}
			free_array(envp);
			free(pids);
			shell->exit_status = 1;
			set_prompt_signals();
			return (1);
		}
		if (pids[i] == 0)
		{
			set_child_signals();
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
				{
					perror("dup2");
					close(prev_fd);
					if (cmds->next)
					{
						close(pipefd[0]);
						close(pipefd[1]);
					}
					free_child_pipeline(shell, paths, envp, pids, tokens);
					exit(1);
				}
				close(prev_fd);
				prev_fd = -1;
			}
			if (cmds->next)
			{
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2");
					if (prev_fd != -1)
						close(prev_fd);
					close(pipefd[0]);
					close(pipefd[1]);
					free_child_pipeline(shell, paths, envp, pids, tokens);
					exit(1);
				}
				close(pipefd[1]);
				close(pipefd[0]);
			}
			if (apply_redirection(cmds->redirects))
			{
				free_child_pipeline(shell, paths, envp, pids, tokens);
				exit(1);
			}
			if (is_builtin(cmds->args))
			{
				shell->in_pipe = 1;
				builtin_status = builtin_functions_in_pipe(shell, tokens, cmds);
				if (builtin_status != 2)
					shell->exit_status = builtin_status;
				free_child_pipeline(shell, paths, envp, pids, tokens);
				exit(shell->exit_status);
			}
			else if (external_command_in_pipe(paths, cmds, shell, envp))
			{
				free_child_pipeline(shell, paths, envp, pids, tokens);
				exit(shell->exit_status);
			}
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmds->next)
			{
				close(pipefd[1]);
				prev_fd = pipefd[0];
			}
			else
				prev_fd = -1;
		}
		cmds = cmds->next;
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (wait_for_pid(pids[i], &status))
		{
			if (i == count -1)
				shell->exit_status = 1;
		}
		if (i == count - 1)
			set_wait_status(shell, status);
		i++;
	}
	set_prompt_signals();
	free_array(envp);
	free(pids);
	return (0);
}
