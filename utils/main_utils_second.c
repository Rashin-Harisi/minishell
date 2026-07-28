/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 main_utils_second.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 11:29:19 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 11:29:19 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

int	handle_token_error(t_main *data)
{
	ft_putstr_fd("Syntax error quotation\n", STDERR_FILENO);
	data->shell.exit_status = 2;
	rl_on_new_line();
	return (1);
}

int	handle_syntax_error(t_main *data)
{
	ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(data->shell.syntax_check, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	data->shell.exit_status = 2;
	rl_on_new_line();
	return (1);
}

int	create_command_list(t_main *data)
{
	data->shell.cmds = create_cmds(data->tokens);
	if (!data->shell.cmds)
	{
		ft_putstr_fd("cmds creation fail\n", STDERR_FILENO);
		data->shell.exit_status = 1;
		return (1);
	}
	if (expansion(&data->shell))
	{
		data->shell.exit_status = 1;
		return (1);
	}
	return (0);
}

int	prepare_commands(t_main *data)
{
	data->tokens = create_tokens(data->shell.line);
	if (!data->tokens)
		return (handle_token_error(data));
	if (syntax_check(data->tokens, &data->shell))
		return (handle_syntax_error(data));
	if (expansion_tokens(data->tokens, &data->shell))
	{
		data->shell.exit_status = 1;
		return (1);
	}
	if (create_command_list(data))
		return (1);
	if (heredoc_preparation(data->shell.cmds, &data->shell))
	{
		data->shell.exit_status = 1;
		return (1);
	}
	return (0);
}
