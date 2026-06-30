#include "minishell.h"

void    free_array(char **args)
{
    int i;

    i = 0;
    if (!args) return;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

void    free_redirects(t_redir *redir)
{
    t_redir *cur;

    while(redir)
    {
        cur = redir->next;
        if (redir->type == REDIR_HEREDOC && redir->fd >= 0)
            close(redir->fd);
        free(redir->filename);
        free(redir);
        redir = cur;
    }
}

void    free_one_cmd(t_cmd *cmd)
{
    if (!cmd) return;
    free_array(cmd->args);
    free_redirects(cmd->redirects);
    free(cmd);    
}

void    free_cmds(t_cmd *cmds)
{
    t_cmd *cur;

    while (cmds)
    {
        cur = cmds->next;
        free_one_cmd(cmds);
        cmds = cur;
    }
}


void    ft_lstadd_back_cmds(t_cmd **cmds, t_cmd *node)
{
    t_cmd	*current;

	if (!cmds || !node)
		return ;
	if (*cmds == NULL)
		*cmds = node;
	else
	{
		current = *cmds;
		while (current->next != NULL)
			current = current->next;
		current->next = node;
	}
}

void    ft_lstadd_back_redirects(t_redir **redir, t_redir *node)
{
    t_redir	*current;

	if (!redir || !node)
		return ;
	if (*redir == NULL)
		*redir = node;
	else
	{
		current = *redir;
		while (current->next != NULL)
			current = current->next;
		current->next = node;
	}
}

t_redir *init_redirect(t_token *tokens)
{
    t_redir *tmp;

    if (!tokens || !tokens->next || !tokens->next->value)
        return (NULL);
    tmp = malloc(sizeof(t_redir));
    if (!tmp) return (NULL);
    tmp->fd = -1;
    tmp->filename = ft_strdup(tokens->next->value);
    if (!tmp->filename) return(free(tmp), NULL);
    if (tokens->type == TOKEN_APPEND)
        tmp->type = REDIR_APPEND;
    else if (tokens->type == TOKEN_HEREDOC)
        tmp->type = REDIR_HEREDOC;
    else if (tokens->type == TOKEN_REDIR_IN)
        tmp->type = REDIR_IN;
    else if (tokens->type == TOKEN_REDIR_OUT)
        tmp->type = REDIR_OUT;
    else
    {
        free(tmp->filename);
        free(tmp);
        return (NULL);
    }
    tmp->next = NULL;
    return (tmp);
} 

int count_args(t_token *tokens)
{
    t_token *tmp;
    int count;

    count = 0;
    tmp = tokens;
    while(tmp && tmp->type != TOKEN_PIPE)
    {
        if ((tmp->type == TOKEN_APPEND ||
            tmp->type == TOKEN_HEREDOC ||
            tmp->type == TOKEN_REDIR_IN||
            tmp->type == TOKEN_REDIR_OUT) && tmp->next)
        {
            tmp = tmp->next->next;
            continue;
        }
        if (tmp->type == TOKEN_WORD)
            count++;
        tmp = tmp->next;
    }
    return (count);
}