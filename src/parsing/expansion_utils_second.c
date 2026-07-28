/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 expansion_utils_second.c							:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/28 14:30:40 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/28 14:30:40 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

int	is_specific_char(char s)
{
	if ((s >= 'a' && s <= 'z')
		|| (s >= 'A' && s <= 'Z')
		|| (s >= '0' && s <= '9')
		|| s == '_')
		return (1);
	else
		return (0);
}

char	*find_var_value(char *var, t_env *envs)
{
	while (envs)
	{
		if (ft_strncmp(var, envs->key, ft_strlen(var) + 1) == 0)
			return (envs->value);
		envs = envs->next;
	}
	return (NULL);
}

char	*append_str(char *str, char *s)
{
	char	*new;

	if (!s)
		s = "";
	new = ft_strjoin(str, s);
	free(str);
	return (new);
}

char	*append_char(char *str, char s)
{
	char	new[2];

	new[0] = s;
	new[1] = '\0';
	return (append_str(str, new));
}

char	*expand_exit_status(t_shell *shell, char *expanded)
{
	char	*status;

	status = ft_itoa(shell->exit_status);
	if (!status)
		return (free(expanded), NULL);
	expanded = append_str(expanded, status);
	free(status);
	return (expanded);
}
