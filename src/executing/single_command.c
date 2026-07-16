#include "minishell.h"


int wait_for_pid(pid_t pid, int *status)
{
    pid_t result;

    while(1)
    {
        result = waitpid(pid, status, 0);
        if (result == pid)
            return (0);
        if (result == -1 && errno == EINTR)
            continue;
        if (result == -1)
        {
            perror("waitpid");
            return (1);
        }
    }
}

void set_wait_status(t_shell *shell, int status)
{
    int sig;

    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        sig = WTERMSIG(status);
        shell->exit_status = 128 + sig;
        if (sig == SIGINT)
            write(STDOUT_FILENO, "\n", 1);
        else if (sig == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
    }
}

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
    set_parent_wait_signals();
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        set_prompt_signals();
        free(pathname);
        shell->exit_status = 1;
        return (1);
    }
    if (pid == 0)
    {
        set_child_signals();
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
        free_cmds(shell->cmds);
        free_envs(shell->env);
        free(shell->line);
        free_paths(paths);
        exit(126);
    }
    if (wait_for_pid(pid, &status))
    {
        shell->exit_status = 1;
        set_prompt_signals();
        free(pathname);
        return (1);
    }
    set_wait_status(shell, status);
    set_prompt_signals();
    free(pathname);
    return (0);
}

static int	restore_standard_fds(int saved_stdin, int saved_stdout)
{
	int	error;

	error = 0;
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		error = 1;
	}
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		error = 1;
	}
	close(saved_stdin);
	close(saved_stdout);
	return (error);
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
        if (saved_stdin != -1)
            close(saved_stdin);
        if (saved_stdout != -1)
            close(saved_stdout);
        shell->exit_status = 1;
        return (1);
    }
    cmd = shell->cmds;
    if (apply_redirection(cmd->redirects))
    {
        restore_standard_fds(saved_stdin, saved_stdout);
        shell->exit_status = 1;
        return (1);
    }
    ret = builtin_functions(shell, tokens);
    if (restore_standard_fds(saved_stdin, saved_stdout))
    {
        shell->exit_status = 1;
        ret = 1;
    }
    return (ret);
}

int execute_single_command(t_shell *shell, char **paths, t_token **tokens)
{
    t_cmd *cmds;

    cmds = shell->cmds;
    if (!cmds->args || !cmds->args[0]) return (only_redirection(cmds, shell));
    if (is_builtin(cmds->args))
    {
        if (cmds->redirects)
            return (builtin_with_redirection(shell, tokens));
        return (builtin_functions(shell, tokens));
    }
    else
        return (execute_external_command(cmds, paths, shell));
}