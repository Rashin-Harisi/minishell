/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 single_utils_first.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:05:05 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:05:05 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	wait_for_pid(pid_t pid, int *status)
{
	pid_t	result;

	while (1)
	{
		result = waitpid(pid, status, 0);
		if (result == pid)
			return (0);
		if (result == -1 && errno == EINTR)
			continue ;
		if (result == -1)
		{
			perror("waitpid");
			return (1);
		}
	}
}

void	set_wait_status(t_shell *shell, int status)
{
	int	sig;

	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		shell->exit_status = 128 + sig;
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}

void	free_external_child(t_shell *shell, char **paths,
	char **envp, char *pathname)
{
	if (envp)
		free_array(envp);
	free(pathname);
	free_cmds(shell->cmds);
	free_envs(shell->env);
	free(shell->line);
	free_paths(paths);
}

void	exit_external_child(t_shell *shell, char **paths,
	char *pathname, int status)
{
	free_external_child(shell, paths, NULL, pathname);
	exit(status);
}

void	run_external_child(t_cmd *cmd, char **paths,
	t_shell *shell, char *pathname)
{
	char	**envp;

	set_child_signals();
	if (apply_redirection(cmd->redirects))
		exit_external_child(shell, paths, pathname, 1);
	envp = convert_list_to_array(shell->env);
	if (!envp)
		exit_external_child(shell, paths, pathname, 1);
	execve(pathname, cmd->args, envp);
	perror(cmd->args[0]);
	free_external_child(shell, paths, envp, pathname);
	exit(126);
}
