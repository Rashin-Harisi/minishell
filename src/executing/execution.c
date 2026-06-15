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

int count_node(t_cmd *cmds)
{
    int i;
    t_cmd *tmp;

    tmp = cmds;
    i = 0;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

int is_builtin(char **args)
{
    if (!args || !args[0]) return(0);
    if (ft_strncmp(args[0], "cd", 3) == 0 || ft_strncmp(args[0], "exit", 5) == 0 || ft_strncmp(args[0], "env", 4) == 0 || ft_strncmp(args[0], "export", 7) == 0 || ft_strncmp(args[0], "unset", 6) == 0 || ft_strncmp(args[0], "echo", 5) == 0 || ft_strncmp(args[0], "pwd", 4) == 0)
        return (1);
    return (0);
}

char *check_access_pathname(char **paths, char *cmd, t_shell *shell)
{
    int i;
    char *temp;
    char *pathname;

    i = 0;
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        shell->exit_status = 126;
        perror(cmd);
        return (NULL);
    }
    while (paths[i])
    {
        temp = ft_strjoin(paths[i], "/");
        pathname = ft_strjoin(temp, cmd);
        free(temp);
        if (access(pathname, X_OK) == 0)
            return (pathname);
        free(pathname);
        i++;
    }
    shell->exit_status = 127;
    printf("minishell: %s: command not found\n", cmd);
    return (NULL);
}

int calculate_nodes(t_env *env)
{
    t_env   *tmp;
    int     i;

    i = 0;
    tmp = env;
    while(tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}
char    **convert_list_to_array(t_env *env)
{
    char    **envp;
    t_env   *tmp;
    int     i;
    char    *line;

    tmp = env;
    i = 0;
    envp = ft_calloc((calculate_nodes(env) + 1) , sizeof(char *));
    if (!envp) return NULL;
    while(tmp)
    {
        line = ft_strjoin(tmp->key, "=");
        if (!line) return (free_array(envp), NULL);
        envp[i] = ft_strjoin(line, tmp->value);
        free(line);
        if (!envp[i]) return (free_array(envp), NULL);
        i++;
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return (envp);
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
            fd = redir->fd;
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

void execute_external_command(t_cmd *cmd, char **paths, t_shell *shell)
{
    char *pathname;
    pid_t pid;
    int status;
    char    **envp;

    pathname = NULL;
    pathname = check_access_pathname(paths, cmd->args[0], shell);
    if (!pathname)
        return;
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(pathname);
        shell->exit_status = 1;
        return;
    }
    if (pid == 0)
    {
        if (apply_redirection(cmd->redirects))
        {
            free(pathname);
            exit(1);
        }
        envp = convert_list_to_array(shell->env);
        if (!envp)
        {
            free(pathname);
            exit(1);
        }
        execve(pathname, cmd->args, envp);
        perror(cmd->args[0]);
        free_array(envp);
        free(pathname);
        exit(126);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell->exit_status = 128 + WTERMSIG(status);
    }
    free(pathname);
}

void    builtin_with_redirection(t_shell *shell, t_token **tokens)
{
    t_cmd   *cmd;
    int     saved_stdin;
    int     saved_stdout;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1) 
    {
        shell->exit_status = 1;
        return;
    }
    cmd = shell->cmds;
    if (apply_redirection(cmd->redirects))
    {
        if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            shell->exit_status = 1;
            return;
        }
        close(saved_stdin);
        close(saved_stdout);
        shell->exit_status = 1;
        return;
    }
    builtin_functions(shell, tokens);
    if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        exit(1);
    }
    close(saved_stdin);
    close(saved_stdout);
}

void execute_single_command(t_shell *shell, char **paths, t_token **tokens)
{
    t_cmd *cmds;

    cmds = shell->cmds;
    if (!cmds->args || !cmds->args[0]) return;
    if (is_builtin(cmds->args))
        builtin_with_redirection(shell, tokens);
    else
        execute_external_command(cmds, paths, shell);
}

void execute_pipeline(t_cmd *cmds)
{
    (void)cmds;
    printf("pipe is exist\n");
}

int execution(t_shell *shell, t_token **tokens, char **paths)
{
    if (check_redirection(shell->cmds))
        return (1);
    if (count_node(shell->cmds) == 1)
        execute_single_command(shell, paths, tokens);
    else
        execute_pipeline(shell->cmds);
    return (0);
}