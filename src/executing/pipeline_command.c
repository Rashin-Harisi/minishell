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
    //int count;
    //int i;
    int pipefd[2];
    //int prev_fd;
    pid_t   pid1;
    pid_t   pid2;
    int status1;
    int status2;
    char **envp;


    envp = convert_list_to_array(shell->env);
    if (!envp) return;
    //prev_fd = -1;
   
    //i = 0;
    //count = count_node(shell->cmds);

        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            free_array(envp);
            shell->exit_status = 1;
            return;
        }
        pid1 = fork();
        if (pid1 == -1 )
        {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            free_array(envp);
            shell->exit_status = 1;
            return;
        }
        if (pid1 == 0)
        {
            if(dup2(pipefd[1], STDOUT_FILENO) == -1) exit(1);
            close(pipefd[0]);
            close(pipefd[1]);
            if (apply_redirection(shell->cmds->redirects))
            {
                free_array(envp);
                exit(1);
            }
            if (is_builtin(shell->cmds->args))
            {
                builtin_functions_in_pipe(shell, tokens, shell->cmds);
                free_array(envp);
                exit(shell->exit_status);
            }
            else
                external_command_in_pipe(paths, shell->cmds, shell, envp);  
        }
        pid2 = fork();
        if (pid2 == -1 )
        {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            free_array(envp);
            shell->exit_status = 1;
            return;
        }
        if (pid2 == 0)
        {
            if (dup2(pipefd[0], STDIN_FILENO) == -1) exit(1);
            close(pipefd[0]);
            close(pipefd[1]);
            if (apply_redirection(shell->cmds->next->redirects))
            {
                free_array(envp);
                exit(1);
            }
            if (is_builtin(shell->cmds->next->args))
            {
                builtin_functions_in_pipe(shell, tokens, shell->cmds->next);
                free_array(envp);
                exit(shell->exit_status);
            }
            else
                external_command_in_pipe(paths, shell->cmds->next, shell, envp);
            
        }
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, &status1, 0);
        waitpid(pid2, &status2, 0);
        if (WIFEXITED(status2))
            shell->exit_status = WEXITSTATUS(status2);
        else if (WIFSIGNALED(status2))
            shell->exit_status = 128 + WTERMSIG(status2);
        free_array(envp);
}