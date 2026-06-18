#include "minishell.h"

void    prepare_reading_pipe(t_redir *redir)
{
    char *line;
    int fd_pipe[2];

    if (!redir || !redir->filename) return;
    if (pipe(fd_pipe) == -1)
    {
        perror("pipe");
        redir->fd = -1;
        return;
    }
    while (1)
    {
        line = readline("heredoc> ");
        if (!line) break ;
        if (ft_strncmp(line, redir->filename, ft_strlen(redir->filename) + 1) == 0)
        {
            free(line);
            break ;
        }
        write(fd_pipe[1], line, ft_strlen(line));
        write(fd_pipe[1], "\n" , 1);
        free(line);
    }
    close(fd_pipe[1]);
    redir->fd = fd_pipe[0];
}

void    heredoc_preparation(t_cmd *cmds)
{
    t_cmd   *tmp;
    t_redir *redir;

    tmp = cmds;
    while (tmp)
    {
        redir = tmp->redirects;
        while (redir)
        {
            if(redir->type == REDIR_HEREDOC)
                prepare_reading_pipe(redir);
            redir = redir->next;
        }
        tmp = tmp->next;
    }
}