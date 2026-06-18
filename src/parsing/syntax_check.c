#include "minishell.h"

int pipe_validation(t_token *prev, t_token *curr)
{
    t_token *next;

    if (!curr) return (0);
    next = curr->next;
    if (next == NULL || prev == NULL) return (0);
    if (prev->type == TOKEN_PIPE || next->type == TOKEN_PIPE) return (0);
    if (prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC
        || prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT)
        return (0);
    return (1);
}

int redir_validation(t_token *curr)
{
    t_token *next;

    if (!curr) return (0);
    next = curr->next;
    if (next == NULL) return (0);
    if (next->type != TOKEN_WORD)
        return (0);
    return (1);
}

int has_forbidden_chart(t_token *curr)
{
    int len;

    if (!curr || !curr->value) return (1);
    len = ft_strlen(curr->value);
    if (ft_strncmp(curr->value, ";" , 2) == 0 || curr->value[len -1] == ';')
        return (1);
    if (ft_strncmp(curr->value, "&" , 2) == 0 || curr->value[len -1] == '&')
        return (1);
    if (ft_strncmp(curr->value, "`" , 2) == 0 || curr->value[len -1] == '`')
        return (1);
    if ((curr->value[0] == '(' && curr->value[len - 1] == ')')
            || (curr->value[0] == '(' || curr->value[len - 1] == ')'))
        return (1);
    return (0);
}

int syntax_check(t_token *tokens)
{

    t_token *prev;
    t_token *curr;

    prev = NULL;
    curr = tokens;
    while(curr)
    {
        if (has_forbidden_chart(curr)) return (1);
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