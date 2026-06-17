#include "minishell.h"

void    external_command_in_pipe(char **paths, t_cmd *cmd, t_shell *shell, char **envp)
{
    char *pathname;

    pathname= NULL;
    pathname = check_access_pathname(paths, cmd->args[0], shell);
    if (!pathname)
    {
        free_array(envp);
        exit(shell->exit_status);
    }
    execve(pathname, cmd->args, envp);
    perror(cmd->args[0]);
    free_array(envp);
    free(pathname);
    exit(126); 
}

void builtin_functions_in_pipe(t_shell *shell, t_token **tokens, t_cmd *cmd)
{
    if (!shell || !cmd || !cmd->args || !cmd->args[0]) return;
    builtin_update_env(shell, cmd);
    if (ft_strncmp(cmd->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
    {
        if (pwd_func(shell))
            printf("minishell : pwd: error\n");
    }
    if (ft_strncmp(cmd->args[0], "echo", ft_strlen("echo") + 1) == 0)
    {
        if(echo_func(shell, cmd))
            printf("minishell : echo: error\n");
    }
    if (ft_strncmp(cmd->args[0], "exit", ft_strlen("exit") + 1) == 0)
    {
        if(exit_func(shell, *tokens, cmd))
            printf("minishell : exit : error\n");
    }
}
void execute_pipeline(t_shell *shell, char **paths, t_token **tokens)
{
    int pipefd[2];
    int prev_fd;
    pid_t   *pids;
    int status;
    char **envp;
    t_cmd   *cmds;
    int count;
    int i;
    
    cmds = shell->cmds;
    envp = convert_list_to_array(shell->env);
    if (!envp) return;
    count = count_node(cmds);
    pids = malloc((count)* sizeof(pid_t));
    if (!pids) return (free_array(envp));
    prev_fd = -1;
    i = 0;
    while(cmds)
    {
        if (cmds->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            free_array(envp);
            free(pids);
            if (prev_fd != -1) close(prev_fd);
            shell->exit_status = 1;
            return;
        }
        pids[i] = fork();
        if (pids[i] == -1 )
        {
            perror("fork");
            if (prev_fd != -1) close(prev_fd);
            if (cmds->next)
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }
            free_array(envp);
            free(pids);
            shell->exit_status = 1;
            return;
        }
        if (pids[i] == 0)
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd , STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }
            if (cmds->next)
            {
                if(dup2(pipefd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(pipefd[1]);
                close(pipefd[0]);
            }
            if (apply_redirection(cmds->redirects))
            {
                free_array(envp);
                free(pids);
                exit(1);
            }
            if (is_builtin(cmds->args))
            {
                builtin_functions_in_pipe(shell, tokens, cmds);
                free_array(envp);
                free(pids);
                exit(shell->exit_status);
            }
            else
                external_command_in_pipe(paths, cmds, shell, envp);  
        }
        else
        {
            if (prev_fd != -1) close(prev_fd);
            if (cmds->next)
            {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
            else
                prev_fd = -1;
        }
        cmds = cmds->next;
        i++;
    }
    i = 0;
    while (i < count)
    {
        waitpid(pids[i], &status, 0);
        if (i == count)
        {
            if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                shell->exit_status = 128 + WTERMSIG(status);
        }
        i++;
    }
    free_array(envp);
    free(pids);
}