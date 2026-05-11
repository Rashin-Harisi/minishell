#include "minishell.h"

t_cmd   *init_cmds(t_token *tokens)
{
    t_cmd   *cmds;
    int     i;
    t_cmd   *tmp;

    i = 0;
    cmds = NULL;
    while (tokens[i].next != NULL)
    {
        tmp = malloc(sizeof(cmds));
        if (!tmp) return (NULL);
        tmp->args = tokens[i].value;
        
        tmp->redirects = malloc(sizeof(t_redir));
        if (!tmp->redirects) return (NULL);
        tmp->redirects->type
    }
}