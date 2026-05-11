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

void	tokentoargs(t_token	*input)
{
	t_cmd	cmds;
	t_redir	redirs;
}
// Note: While loop. If token -> word, ->args, if pipe -> next, 
// Cont: else -> redirects, change type accordingly. When execve, no next->done. 
// For HEREDOCS, order goes from left -> right. If no pipe, only last matters. If pipe, each takes input and puts them into pipe.