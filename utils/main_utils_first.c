/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 main_utils_first.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 11:27:56 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 11:27:56 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

void	free_iteration(t_token *tokens, t_cmd *cmds, char *line)
{
	if (cmds)
		free_cmds(cmds);
	if (tokens)
		free_tokens(tokens);
	if (line)
		free(line);
}

void	remove_empty_args(char **args)
{
	int	i;
	int	j;

	if (!args)
		return ;
	i = 0;
	j = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
		{
			args[j] = args[i];
			j++;
		}
		else
			free(args[i]);
		i++;
	}
	args[j] = NULL;
}

void	init_main(t_main *data, char **envp)
{
	ft_memset(data, 0, sizeof(data));
	if (envp[0] == NULL)
		data->shell.env = create_minimal_envp();
	else
		data->shell.env = init_env(envp);
	data->paths = NULL;
	data->shell.interactive = isatty(STDIN_FILENO);
	data->shell.exit_status = 0;
}

char	*read_shell_line(t_shell *shell)
{
	char	*prompt;
	char	*line;

	if (shell->interactive)
	{
		prompt = create_prompt(shell);
		line = readline(prompt);
		free(prompt);
		return (line);
	}
	line = get_next_line(STDIN_FILENO);
	if (line && ft_strlen(line) > 0
		&& line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (line);
}
