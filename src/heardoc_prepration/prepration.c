#include "minishell.h"

void    prepare_reading_pipe(t_redir *redir)
{
    char *line;
    char *prompt;
    int fd_pipe[2];
    int dup_fd;
    char buffer[1024];
    ssize_t bytes;

    if (pipe(fd_pipe) == -1) return;
    dup_fd = dup(fd_pipe[0]);
    prompt = "heredoc> ";
    while (1)
    {
        line = readline(prompt);
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
    bytes = read(dup_fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        printf("PIPE CONTENT : \n%s\n", buffer);
    }
    close(dup_fd);
    redir->fd = fd_pipe[0];
}

void    heredoc_preparation(t_cmd *cmds)
{
    t_cmd   *tmp;
    t_redir *redir;

    if (!cmds) return;
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