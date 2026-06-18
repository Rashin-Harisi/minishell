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
        cur = redir-> next;
        if (redir->fd != -1)
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

t_cmd *init_cmds(t_token **tokens)
{
    t_cmd   *cmd;
    int     i;
    t_redir *node;
    int count;

    if(!tokens || !*tokens) return (NULL);
    i = 0;
    count = count_args(*tokens);
    cmd = malloc(sizeof(t_cmd));
    if (!cmd) return (NULL);
    cmd->redirects = NULL;
    cmd->args = NULL;
    cmd->next = NULL;
    cmd->args = ft_calloc((count + 1), sizeof(char *));
    if(!cmd->args) return (free(cmd),NULL);
    while(*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if( (*tokens)->type == TOKEN_WORD)
        {
            cmd->args[i] = ft_strdup((*tokens)->value);
            if (!cmd->args[i]) return(free_one_cmd(cmd), NULL);
            i++;
            (*tokens) = (*tokens)->next;
            continue;
        }
        if ((*tokens)->type == TOKEN_APPEND ||
            (*tokens)->type == TOKEN_HEREDOC ||
            (*tokens)->type == TOKEN_REDIR_IN||
            (*tokens)->type == TOKEN_REDIR_OUT)
        {
            node = init_redirect(*tokens);
            if (!node) return (free_one_cmd(cmd), NULL);
            ft_lstadd_back_redirects(&cmd->redirects, node);
            (*tokens) = (*tokens)->next->next;
            continue;
        }
        (*tokens) = (*tokens)->next;
    }
    cmd->args[i] = NULL;
    return (cmd);
}

t_cmd   *create_cmds(t_token *tokens)
{
    t_cmd   *cmds;
    t_cmd   *tmp;

    cmds = NULL;
    while (tokens)
    {
        tmp = init_cmds(&tokens);
        if (!tmp) return(free_cmds(cmds), NULL);
        ft_lstadd_back_cmds(&cmds, tmp);
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    return (cmds);
}