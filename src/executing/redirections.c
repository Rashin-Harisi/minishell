#include "minishell.h"


int check_redirection(t_cmd *cmds)
{
    t_redir *redir;
    t_cmd *cmd;
    int fd;

    cmd = cmds;
    while (cmd)
    {
        redir = cmd->redirects;
        while (redir)
        {
            fd = -1;
            if (redir->type == REDIR_IN)
                fd = open(redir->filename, O_RDONLY);
            else if (redir->type == REDIR_OUT)
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (redir->type == REDIR_HEREDOC)
            {
                if (redir->fd == -1)
                    return (1);
                redir = redir->next;
                continue;
            }
            else if (redir->type == REDIR_APPEND)
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
            {
                redir = redir->next;
                continue;
            }
            if (fd == -1)
            {
                perror(redir->filename);
                return (1);
            }
            close(fd);
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return (0);
}

int only_redirection(t_cmd *cmd, t_shell *shell)
{
    int saved_stdin;
    int saved_stdout;
    int ret;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1)
    {
        if (saved_stdin != -1)
            close(saved_stdin);
        if (saved_stdout != -1)
            close(saved_stdout);
        shell->exit_status = 1;
        return (1);
    }
    ret = apply_redirection(cmd->redirects);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    if (ret)
    {
        shell->exit_status = 1;
        return (1);
    }
    shell->exit_status = 0;
    return (0);
}

int apply_redirection(t_redir *redirects)
{
    t_redir *redir;
    int fd;

    redir = redirects;
    while (redir)
    {
        fd = -1;
        if (redir->type == REDIR_IN)
            fd = open(redir->filename, O_RDONLY);
        else if (redir->type == REDIR_OUT)
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (redir->type == REDIR_HEREDOC)
        {
            fd = redir->fd;
            redir->fd = -1;
        }
        else if (redir->type == REDIR_APPEND)
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
        {
            perror(redir->filename);
            return (1);
        }
        if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
        {
            if (dup2(fd, STDIN_FILENO) == -1)
                return (perror("dup2"),close(fd), 1);
        }
        else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
        {
            if (dup2(fd, STDOUT_FILENO) == -1)
                return (perror("dup2"), close(fd),1);
        }
        close(fd);
        redir = redir->next;
    }
    return (0);
}