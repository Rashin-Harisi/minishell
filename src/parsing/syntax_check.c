#include "minishell.h"

int pipe_validation(t_token *prev, t_token *curr)
{
    t_token *next;

    next = curr->next;
    if (next == NULL || prev == NULL) return (0);
    if (prev->type == TOKEN_PIPE || next->type == TOKEN_PIPE) return (0);
    if (prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC
        || prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT)
        return (0);
    if (next->type == TOKEN_APPEND || next->type == TOKEN_HEREDOC
        || next->type == TOKEN_REDIR_IN || next->type == TOKEN_REDIR_OUT)
        return (0);
    return (1);
}

int redir_validation(t_token *curr)
{
    t_token *next;

    next = curr->next;
    if (next == NULL) return (0);
    if (next->type != TOKEN_WORD)
        return (0);
    return (1);
}

int syntax_check(t_token *tokens)
{

    t_token *prev;
    t_token *curr;

    prev = NULL;
    curr = tokens;
    while(curr)
    {
        if (curr->type == TOKEN_PIPE && !pipe_validation(prev, curr))
            return (1);
        
        if ((curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC
            || curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_REDIR_OUT)
            && !redir_validation(curr))
            return (1);
        prev = curr;
        curr = curr->next;
    }
    return (0);
}