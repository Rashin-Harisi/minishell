/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmoser <pmoser@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 15:37:32 by pmoser            #+#    #+#             */
/*   Updated: 2026/05/11 15:37:41 by pmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_redir(t_token_type token)
{
	if (token == TOKEN_REDIR_IN || token == TOKEN_REDIR_OUT
		|| token == TOKEN_APPEND)
		return (1);
	return (0);
}

int		is_val_assignment(t_token	*token)
{
	int		index;
	char	*str;

	index = 0;
	if (!token || token->type != TOKEN_WORD)
		return (0);
	str = token->value;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	index = 1;
	while (str[index] != '\0' && str[index] != '=')
	{
		if (!ft_isalnum(str[index]) && str[index] != '_')
			return (0);
		index++;
	}
	return (str[index] == '=');
}

void	roleandexpect(t_parser	*parse, t_role	role, t_expect expect)
{
	parse->curr->role = role;
	parse->expect = expect;
}

void	roleassigner(t_parser	*p)
{
	while(p->curr)
	{
		if (p->curr->type == TOKEN_PIPE)
			p->expect = EXPECT_CMD;
		else if (p->expect == EXPECT_CMD)
		{
			if (p->curr->type == TOKEN_WORD && is_val_assignment(p->curr))
				p->curr->role = ROLE_ENV_ASSIGNMENT;
			else if (p->curr->type == TOKEN_WORD)
				roleandexpect(p, ROLE_CMD, EXPECT_CMD_BODY);
		}
		else if (p->expect == EXPECT_CMD_BODY && is_redir(p->curr->type))
			p->expect = EXPECT_FILE;
		else if (p->expect == EXPECT_CMD_BODY && p->curr->type == TOKEN_HEREDOC)
				p->expect = EXPECT_LIMITER;
		else if (p->expect == EXPECT_CMD_BODY && p->curr->type == TOKEN_WORD)
			roleandexpect(p, ROLE_ARG, EXPECT_CMD_BODY);
		else if (p->expect == EXPECT_LIMITER && p->curr->type == TOKEN_WORD)
			roleandexpect(p, ROLE_LIMITER, EXPECT_CMD_BODY);
		else if (p->expect == EXPECT_FILE && p->curr->type == TOKEN_WORD)
			roleandexpect(p, ROLE_FILE, EXPECT_CMD_BODY);
		else
			return (p->error = -1);
		p->curr = p->curr->next;
	}
} 
//ASSIGN ROLE AND EXPECT BEFORE THIS FUNCTION!! THIS IS ONLY FOR ASSIGNMENT!!

int		prestalloc(t_token *input)
{
	int		count;
	t_token	*ph;

	count = 0;
	ph = input;
	while (ph->next != NULL && ph->next->type != TOKEN_PIPE)
	{
		if (ph->type == TOKEN_WORD)
			count++;
		ph = ph->next;
	}
	if (ph->type == TOKEN_WORD)
		count++;
	return (count);
}
static t_token_type	tokentoredir(t_token_type	type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (type == TOKEN_APPEND)
		return (REDIR_APPEND);
	else if (type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
}

int		argalloc(t_token *input, t_cmd *cmds)
{
	int		countarg;
	int		index;
	t_token	*ph;

	countarg = 0;
	ph = input;
	cmds->args = malloc(sizeof(char *) * (prestalloc(input) + 1));
	while (ph != NULL)
	{
		if (ph->type == TOKEN_WORD)
		{
			if (ph->role == ROLE_CMD || ph->role == ROLE_ARG)
				cmds->args[countarg++] = ft_strdup(ph->value);
			else if (ph->role == ROLE_FILE)
				cmds->redirects->filename = ft_strdup(ph->value);
		}
		else if (ph->type == is_redir(ph->type))
			cmds->redirects->type = tokentoredir(ph->type);
		else if (ph->type == TOKEN_HEREDOC)
			cmds->redirects->type = REDIR_HEREDOC;
		
	}
}

// Note: While loop. If token -> word, ->args, if pipe -> next, 
// Cont: else -> redirects, change type accordingly. When execve, no next->done. 
// For HEREDOCS, order goes from left -> right. If no pipe, only last matters. If pipe, each takes ph and puts them into pipe.