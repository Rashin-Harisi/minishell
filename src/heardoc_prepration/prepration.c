#include "minishell.h"

void    prepare_reading_pipe(t_redir *redir, t_shell *shell)
{
    char *line;
    int fd_pipe[2];
    char    *expanded;

    if (!redir || !redir->filename) return;
    redir->fd = -1;
    if (pipe(fd_pipe) == -1)
    {
        perror("pipe");
        return;
    }
    while (1)
    {
        line = readline("heredoc> ");
        if (!line) break ;
        if (!redir->quoted)
            expanded = expansion_string(line, shell);
        else
            expanded = ft_strdup(line);
        free(line);
        if (!expanded)
                break;
        if (ft_strncmp(expanded, redir->filename, ft_strlen(redir->filename) + 1) == 0)
        {
            free(expanded);
            break ;
        }
        write(fd_pipe[1], expanded, ft_strlen(expanded));
        write(fd_pipe[1], "\n" , 1);
        free(expanded);
        
    }
    close(fd_pipe[1]);
    redir->fd = fd_pipe[0];
}

void    heredoc_preparation(t_cmd *cmds, t_shell *shell)
{
    t_cmd   *tmp;
    t_redir *redir;
    char    *expanded;

    tmp = cmds;
    while (tmp)
    {
        redir = tmp->redirects;
        while (redir)
        {
            if(redir->type == REDIR_HEREDOC)
            {
                expanded = expansion_string(redir->filename, shell);
                if (!expanded)
                    return ;
                free(redir->filename);
                redir->filename = expanded;
                prepare_reading_pipe(redir, shell);
            }
            redir = redir->next;
        }
        tmp = tmp->next;
    }
}