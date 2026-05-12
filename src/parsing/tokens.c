#include "minishell.h"

typedef struct s_flags
{
    int single_quote;
    int double_quote;
} t_flags;

void    skip_spaces(char *line, int *index)
{
    while (line[*index] != '\0')
    {
        if(line[*index] == ' ' || line[*index] == 9)
           (*index)++;
        else
            return;
    }
}

char    *each_part_extract(char *line, int *index, int *syntax_error)
{
    t_flags flags;
    int     start;
    int     len = 0;
    char    *tmp;

    ft_memset(&flags, 0, sizeof(t_flags));
    skip_spaces(line, index);
    if (line[*index] == '\0') return NULL;
    if ((line[*index] == '>' && line[*index + 1] == '>' && !flags.single_quote && !flags.double_quote)
    || (line[*index] == '<' && line[*index + 1] == '<' && !flags.single_quote && !flags.double_quote))
    {
        tmp = malloc(3 * sizeof(char));
        if (!tmp) return NULL;
        tmp[0] = line[*index];
        tmp[1] = line[*index + 1];
        tmp[2] = '\0';
        (*index) += 2;
        return (tmp);
    }
    if ((line[*index] == '>' && !flags.single_quote && !flags.double_quote)
    || (line[*index] == '<' && !flags.single_quote && !flags.double_quote)
    || (line[*index] == '|' && !flags.single_quote && !flags.double_quote))
    {
        tmp = malloc(2 * sizeof(char));
        if (!tmp) return NULL;
        tmp[0] = line[*index];
        tmp[1] = '\0';
        (*index) += 1;
        return (tmp);
    }
    start = *index;
    while(line[*index] != '\0')
    {
        if(line[*index] == '"' && !flags.single_quote) flags.double_quote = !flags.double_quote;
        if(line[*index] == '\'' && !flags.double_quote) flags.single_quote = !flags.single_quote;
        if(line[*index] == ' ' && (!flags.single_quote && !flags.double_quote))
        {
            len = (*index) - start;
            (*index)++;
            break;
        }
        if ((line[*index] == '>' && line[*index + 1] == '>' && !flags.single_quote && !flags.double_quote)
            || (line[*index] == '<' && line[*index + 1] == '<' && !flags.single_quote && !flags.double_quote)
            || (line[*index] == '>' && !flags.single_quote && !flags.double_quote)
            || (line[*index] == '<' && !flags.single_quote && !flags.double_quote)
            || (line[*index] == '|' && !flags.single_quote && !flags.double_quote))
        {
            len = (*index) - start;
            break;
        }
        (*index)++;
    }
    if (flags.double_quote || flags.single_quote)
    {
        (*syntax_error) = 1;
        return(NULL);
    }
    if (len == 0)
        len = (*index) - start;
    tmp = ft_substr(line, start, len);
    return (tmp);
}

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
        if (tokens->value)
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
