#include "minishell.h"

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

    tmp = NULL;
    ft_memset(&flags, 0, sizeof(t_flags));
    skip_spaces(line, index);
    if (line[*index] == '\0') return NULL;
    if ((line[*index] == '>' && line[*index + 1] == '>')
    || (line[*index] == '<' && line[*index + 1] == '<' ))
    {
        tmp = malloc(3 * sizeof(char));
        if (!tmp) return NULL;
        tmp[0] = line[*index];
        tmp[1] = line[*index + 1];
        tmp[2] = '\0';
        (*index) += 2;
        return (tmp);
    }
    if ((line[*index] == '>')
    || (line[*index] == '<')
    || (line[*index] == '|'))
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
        if(line[*index] == '"' && !flags.single_quote)
            flags.double_quote = !flags.double_quote;
        if(line[*index] == '\'' && !flags.double_quote)
            flags.single_quote = !flags.single_quote;
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