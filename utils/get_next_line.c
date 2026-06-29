#include "minishell.h"

char *extract_line(char *s, int c)
{
    int i = 0;
    char *tmp;
    if (!s || !*s) return (NULL);
    while (s[i] && s[i] != c)
        i++;
    if (s[i] == c)
        i++;
    tmp = malloc((i +1) * sizeof(char));
    if (!tmp)
        return NULL;
    tmp[i] = '\0';
    while (i > 0)
    {
        i--;
        tmp[i] = s[i];
    }
    return tmp;
}

void *ft_memcpy(void *dest, const void *src, size_t n)
{
    while (n > 0)
    {
        ((char *)dest)[n - 1] = ((char *)src)[n - 1];
        n--;
    }
    return dest;
}

int str_append_mem(char **s1, char *s2, size_t size2)
{
    if (!*s1)
    {
        *s1 = malloc(1 * sizeof(char));
        if(!*s1)
            return 0;
        (*s1)[0] = '\0';
    }
    size_t size1 = (size_t)ft_strlen(*s1);
    char *tmp = malloc((size2 + size1 + 1) * sizeof(char));
    if (!tmp)
        return 0;
    ft_memcpy(tmp, *s1, size1);
    ft_memcpy(tmp + size1 , s2, size2);
    tmp[size1 + size2] = '\0';
    free(*s1);
    *s1 = tmp;
    return 1;
}

int str_append_str(char **s1, char *s2)
{
    return str_append_mem(s1, s2, ft_strlen(s2));
}

void *ft_memmove(void *dest, const void *src, size_t n)
{
    size_t i = 0;
    if (dest == src || n == 0) return (dest);
    if (dest < src)
    {
        while (i < n)
        {
            ((char *)dest)[i] = ((char *)src)[i];
            i++;
        }
    }
    else if (dest > src)
    {
        while (n > 0)
        {
            ((char *)dest)[n - 1] = ((char *)src)[n - 1];
            n--;
        }
    }
    return dest;
}

int new_line_exist(char *line)
{
    int i = 0;
    if (!line) return 0;
    while (line[i])
    {
        if (line[i] == '\n')
            return 1;
        i++;
    }
    return 0;
}

char *get_next_line(int fd)
{
    char *line;
    static char *b;
    int read_ret;
    char *ret;

    if (fd < 0 || BUFFER_SIZE <= 0) return NULL;
    if (!b)
        b = NULL;
    ret = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!ret)
        return (NULL);
    read_ret = 1;
    while (!new_line_exist(b) && read_ret > 0)
    {
        read_ret = read(fd, ret, BUFFER_SIZE);
        if (read_ret < 0)
        {
            free(ret);
            return (NULL);
        }
        ret[read_ret] = '\0';
        if (read_ret > 0 && !str_append_mem(&b, ret, read_ret))
        {
            free(ret);
            return (NULL);
        }
    }
    free(ret);
    if (!b || *b == '\0')
        return NULL;
    line = extract_line(b, '\n');
    if (!line) return NULL;
    size_t len_b = (size_t)ft_strlen(b);
    size_t len_l= (size_t)ft_strlen(line);
    ft_memmove(b, b + len_l, len_b - len_l + 1);
    if (*b == '\0')
    {
        free(b);
        b = NULL;
    }
    return (line);
}
