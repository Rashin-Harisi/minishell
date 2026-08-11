/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 utils_second.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/30 18:14:01 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/30 18:14:01 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	handle_heredoc_sigint(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	set_heredoc_signals(void)
{
	set_signal_action(SIGINT, handle_heredoc_sigint);
	set_signal_action(SIGQUIT, SIG_IGN);
}

int	run_heredoc(int fd, t_redir *redir, t_shell *shell)
{
	int	stdin_backup;
	int	read_status;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (1);
	set_heredoc_signals();
	read_status = read_heredoc(fd, redir, shell);
	if (dup2(stdin_backup, STDIN_FILENO) == -1)
		read_status = 1;
	close(stdin_backup);
	set_prompt_signals();
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		shell->exit_status = 130;
		return (1);
	}
	return (read_status);
}
