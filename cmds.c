//#include "minishell.h"
#include <stdio.h>
#include "libft/libft.h"

int     string_count(char *line)
{
    int i = 0;
    int quot_flag = 0;
    int count = 0;

    while (line[i] != '\0')
    {
        while (line[i] != ' ')
        {
            if (line[i] == '"') !quot_flag;
            i++;
        }
        if (line[i] == ' ' && quot_flag)
       {
            i++;
        }
        else if (line[i] == ' ' && !quot_flag)
        {
            count++;
            i++;
        }
    }
    return count;
}

int sub_stirng_count(char * line)
{
    int i = 0;
    int quot_flag = 0;
    int count = 0;

    while (line[i] != '\0')
    {
        while (line[i] != ' ')
        {
            if (line[i] == '"') !quot_flag;
            i++;
            count++;
        }
        if (line[i] == ' ' && quot_flag)
        {
            count++;
            i++;
        }
        else if (line[i] == ' ' && !quot_flag)
        {
            break;
        }
    }
    return count;
}

void	*free_handler(char **ptr, unsigned int *i_ptr)
{
	unsigned int	index;

	index = 0;
	while (index < *i_ptr)
	{
		free(ptr[index]);
		index++;
	}
	free(ptr);
	return (NULL);
}



char    **init_cmds(char *line)
{
    char    **strings;
    int     quot_flag; // 1 means no open quotation, -1 means open quotation
    int     index;
    int     sub_index;
    int     i = 0;
    int     count = 0;


    index = 0;
    sub_index = 0;
    quot_flag = 0;
    count = string_count(line);
    strings = malloc((count + 1) * sizeof(char *));
    if (strings == NULL) return NULL;
    while (index < count)
    {
        int subcount = sub_stirng_count(line);
        strings[index] = malloc((subcount + 1) * sizeof(char));
        if (!strings[index]) return (free_handler(strings, (unsigned int)index));
        strings[index] = ft_strlcpy()
        index++;
    }

    return (strings);
}

int main(void)
{
    char    *test = "echo \"hello world\"";
    char    **strings = init_cmds(test);
    int i = 0;
    while (strings[i])
    {
        printf("strings[%d] : %s", i, strings[i]);
        i++;
    }

}