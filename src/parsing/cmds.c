#include "minishell.h"


void    free_cmds(t_cmd *cmds)
{
}

void    ft_lstadd_back_cmds(t_cmd **cmds, t_cmd *node)
{}

t_cmd *init_cmds(t_token **tokens)
{}

t_cmd   *create_cmds(t_token *tokens)
{
    t_cmd   *cmds;
    t_cmd   *tmp;

    cmds = NULL;
    while (tokens)
    {
        tmp = malloc(sizeof(t_cmd));
        if(!tmp) return (free_cmds(cmds), NULL);
        tmp = init_cmds(&tokens);
        ft_lstadd_back_cmds(&cmds, tmp);
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    return (cmds);
}