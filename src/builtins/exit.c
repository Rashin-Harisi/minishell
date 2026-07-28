/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 exit.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 09:51:37 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 09:51:37 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	is_valid_arg(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i] != '\0')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	print_exit_message(t_shell *shell)
{
	if (shell->interactive && !shell->in_pipe)
		ft_putstr_fd("exit\n", STDERR_FILENO);
}

int	handle_numeric_error(t_shell *shell, char *arg)
{
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	print_exit_message(shell);
	shell->exit_status = 2;
	return (2);
}

int	handle_too_many_args(t_shell *shell)
{
	ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
	shell->exit_status = 1;
	return (1);
}

int	exit_func(t_shell *shell, t_token *tokens, t_cmd *cmd)
{
	int	code;

	(void)tokens;
	if (!cmd->args[1])
	{
		print_exit_message(shell);
		return (2);
	}
	if (!is_valid_arg(cmd->args[1]))
		return (handle_numeric_error(shell, cmd->args[1]));
	if (cmd->args[2])
		return (handle_too_many_args(shell));
	print_exit_message(shell);
	code = ft_atoi(cmd->args[1]);
	shell->exit_status = code;
	return (2);
}
