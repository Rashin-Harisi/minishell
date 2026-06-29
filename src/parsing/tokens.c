#include "minishell.h"

t_token    *init_token(char *tmp, t_token *node)
{
    node->value = tmp;
    if (tmp[0] == '>' && tmp[1] == '>' && tmp[2] == '\0')
        node->type = TOKEN_APPEND;
    else if (tmp[0] == '<' && tmp[1] == '<' && tmp[2] == '\0')
        node->type = TOKEN_HEREDOC;
    else if (tmp[0] == '<' && tmp[1] == '\0')
        node->type = TOKEN_REDIR_IN;
    else if (tmp[0] == '>' && tmp[1] == '\0')
        node->type = TOKEN_REDIR_OUT;
    else if (tmp[0] == '|' && tmp[1] == '\0')
        node->type = TOKEN_PIPE;
    else
        node->type = TOKEN_WORD;
    node->next = NULL;
    return (node);
}

void	ft_lstadd_back_token(t_token **token, t_token *new)
{
	t_token	*current;

	if (!token || !new)
		return ;
	if (*token == NULL)
		*token = new;
	else
	{
		current = *token;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

void    free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}

t_token   *create_tokens(char *line)
{
    t_token *tokens;
    t_token *node;
    char    *tmp;
    int     i;
    int     syntax_error = 0;

    i = 0;
    tokens = NULL;
    tmp = each_part_extract(line, &i , &syntax_error);
    while (tmp != NULL  && !syntax_error)
    {
        node = malloc(sizeof(t_token));
        if (!node)
        {
            free(tmp);
            free_tokens(tokens);
            return NULL;
        }
        init_token(tmp, node);
        ft_lstadd_back_token(&tokens, node);
        tmp = each_part_extract(line, &i , &syntax_error);
    }
    if (syntax_error)
    {
        free_tokens(tokens);
        return (NULL);
    }
    return (tokens);
}
