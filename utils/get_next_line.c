/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmoser <pmoser@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 04:54:45 by pmoser            #+#    #+#             */
/*   Updated: 2026/07/28 12:33:32 by rabdolho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*extract_line(char *s, int c)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!s || !*s)
		return (NULL);
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == c)
		i++;
	tmp = malloc((i +1) * sizeof(char));
	if (!tmp)
		return (NULL);
	tmp[i] = '\0';
	while (i > 0)
	{
		i--;
		tmp[i] = s[i];
	}
	return (tmp);
}

static int	str_append_mem(char **s1, char *s2, size_t size2)
{
	char	*tmp;
	size_t	size1;

	if (!*s1)
	{
		*s1 = malloc(1 * sizeof(char));
		if (!*s1)
			return (0);
		(*s1)[0] = '\0';
	}
	size1 = (size_t)ft_strlen(*s1);
	tmp = malloc((size2 + size1 + 1) * sizeof(char));
	if (!tmp)
		return (0);
	ft_memcpy(tmp, *s1, size1);
	ft_memcpy(tmp + size1, s2, size2);
	tmp[size1 + size2] = '\0';
	free(*s1);
	*s1 = tmp;
	return (1);
}

static int	new_line_exist(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i])
	{
		if (line[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static int	read_to_buff(char **b, int fd)
{
	char	*ret;
	int		read_ret;

	ret = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!ret)
		return (0);
	read_ret = 1;
	while (!new_line_exist(*b) && read_ret > 0)
	{
		read_ret = read(fd, ret, BUFFER_SIZE);
		if (read_ret < 0)
			return (free(ret), NULL);
		ret[read_ret] = '\0';
		if (read_ret > 0 && !str_append_mem(&b, ret, read_ret))
			return (free(ret), NULL);
	}
	free(ret);
	return (1);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*b;
	size_t		len_b;
	size_t		len_l;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!b)
		b = NULL;
	if (!read_to_buff(&b, fd))
		return (NULL);
	if (!b || *b == '\0')
		return (NULL);
	line = extract_line(b, '\n');
	if (!line)
		return (NULL);
	len_b = (size_t)ft_strlen(b);
	len_l = (size_t)ft_strlen(line);
	ft_memmove(b, b + len_l, len_b - len_l + 1);
	if (*b == '\0')
	{
		free(b);
		b = NULL;
	}
	return (line);
}
