/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 tokens.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: rabdolho <rabdolho@student.42vienna.com>	+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/07/27 11:27:25 by rabdolho		   #+#	  #+#			  */
/*	 Updated: 2026/07/27 11:27:25 by rabdolho		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "minishell.h"

t_token	*create_token_node(char *value)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	return (init_token(value, node));
}

int	add_token(t_token **tokens, char *value)
{
	t_token	*node;

	node = create_token_node(value);
	if (!node)
	{
		free(value);
		return (1);
	}
	ft_lstadd_back_token(tokens, node);
	return (0);
}

int	fill_tokens(char *line, t_token **tokens, int *syntax_error)
{
	char	*tmp;
	int		index;

	index = 0;
	tmp = each_part_extract(line, &index, syntax_error);
	while (tmp && !*syntax_error)
	{
		if (add_token(tokens, tmp))
			return (1);
		tmp = each_part_extract(line, &index, syntax_error);
	}
	return (0);
}

t_token	*create_tokens(char *line)
{
	t_token	*tokens;
	int		syntax_error;

	tokens = NULL;
	syntax_error = 0;
	if (fill_tokens(line, &tokens, &syntax_error))
		return (free_tokens(tokens), NULL);
	if (syntax_error)
		return (free_tokens(tokens), NULL);
	return (tokens);
}
