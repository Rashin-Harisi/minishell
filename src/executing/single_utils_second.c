/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 single_utils_second.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:05:58 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:05:58 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	handle_fork_error(t_shell *shell, char *pathname)
{
	perror("fork");
	set_prompt_signals();
	free(pathname);
	shell->exit_status = 1;
	return (1);
}

int	wait_external_child(t_shell *shell, pid_t pid, char *pathname)
{
	int	status;

	if (wait_for_pid(pid, &status))
	{
		shell->exit_status = 1;
		set_prompt_signals();
		free(pathname);
		return (1);
	}
	set_wait_status(shell, status);
	set_prompt_signals();
	free(pathname);
	return (0);
}

int	execute_external_command(t_cmd *cmd, char **paths, t_shell *shell)
{
	char	*pathname;
	pid_t	pid;

	pathname = check_access_pathname(paths, cmd->args[0], shell);
	if (!pathname)
		return (1);
	set_parent_wait_signals();
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(shell, pathname));
	if (pid == 0)
		run_external_child(cmd, paths, shell, pathname);
	return (wait_external_child(shell, pid, pathname));
}

int	restore_standard_fds(int saved_stdin, int saved_stdout)
{
	int	error;

	error = 0;
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		error = 1;
	}
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		error = 1;
	}
	close(saved_stdin);
	close(saved_stdout);
	return (error);
}

int	save_standard_fds(int saved[2])
{
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] != -1 && saved[1] != -1)
		return (0);
	if (saved[0] != -1)
		close(saved[0]);
	if (saved[1] != -1)
		close(saved[1]);
	return (1);
}
