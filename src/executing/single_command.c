#include "minishell.h"

int execute_external_command(t_cmd *cmd, char **paths, t_shell *shell)
{
    char *pathname;
    pid_t pid;
    int status;
    char    **envp;

    pathname = NULL;
    pathname = check_access_pathname(paths, cmd->args[0], shell);
    if (!pathname)
        return (1);
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(pathname);
        shell->exit_status = 1;
        return (1);
    }
    if (pid == 0)
    {
        if (apply_redirection(cmd->redirects))
        {
            free(pathname);
            shell->exit_status = 1;
            free_cmds(shell->cmds);
            free_envs(shell->env);
            free(shell->line);
            free_paths(paths);
            exit (1);
        }
        envp = convert_list_to_array(shell->env);
        if (!envp)
        {
            free(pathname);
            shell->exit_status = 1;
            free_cmds(shell->cmds);
            free_envs(shell->env); 
            free(shell->line);
            free_paths(paths);
            exit(1);
        }
        execve(pathname, cmd->args, envp);
        perror(cmd->args[0]);
        free_array(envp);
        free(pathname);
        shell->exit_status = 126;
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
    return (0);
}

int builtin_with_redirection(t_shell *shell, t_token **tokens)
{
    t_cmd   *cmd;
    int     saved_stdin;
    int     saved_stdout;
    int     ret;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1) 
    {
        shell->exit_status = 1;
        return (1);
    }
    cmd = shell->cmds;
    if (apply_redirection(cmd->redirects))
    {
        if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            shell->exit_status = 1;
            close(saved_stdin);
            close(saved_stdout);
            return (1);
        }
        close(saved_stdin);
        close(saved_stdout);
        shell->exit_status = 1;
        return (1);
    }
    ret = builtin_functions(shell, tokens);
    if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        shell->exit_status = 1;
        ret = 1;
    }
    close(saved_stdin);
    close(saved_stdout);
    return (ret);
}

int execute_single_command(t_shell *shell, char **paths, t_token **tokens)
{
    t_cmd *cmds;

    cmds = shell->cmds;
    if (!cmds->args || !cmds->args[0]) return (1);
    if (is_builtin(cmds->args))
        return builtin_with_redirection(shell, tokens);
    else
        return execute_external_command(cmds, paths, shell);
}