#include "minishell.h"

int is_directory(char *path)
{
    struct stat st;

    if (stat(path, &st) == -1)
        return (0);
    return (S_ISDIR(st.st_mode));
}

char *check_access_pathname(char **paths, char *cmd, t_shell *shell)
{
    int i;
    char *temp;
    char *pathname;

    i = 0;
    if (!cmd || !cmd[0]) return (NULL);
    if (ft_strchr(cmd, '/'))
    {
        if (is_directory(cmd))
        {
            ft_putstr_fd(cmd,2);
            ft_putstr_fd(": Is a directory\n", 2);
            shell->exit_status = 126;
            return (NULL);
        }
        if (access(cmd, F_OK) != 0)
        {
            perror(cmd);
            shell->exit_status = 127;
            return (NULL);
        }
        if (access(cmd, X_OK) != 0)
        {
            perror(cmd);
            shell->exit_status = 126;
            return (NULL);
        }
        return (ft_strdup(cmd));
    }
    if (!paths)
    {
        shell->exit_status = 127;
        ft_putstr_fd(cmd ,2);
        ft_putstr_fd(": command not found\n",2);
        return (NULL);
    }
    while (paths[i])
    {
        temp = ft_strjoin(paths[i], "/");
        if (!temp) return (shell->exit_status= 1, NULL);
        pathname = ft_strjoin(temp, cmd);
        free(temp);
        if (!pathname) return (shell->exit_status= 1, NULL);
        if (access(pathname, F_OK) == 0)
        {
            if (is_directory(pathname))
            {
                ft_putstr_fd(cmd, 2);
                ft_putstr_fd(": Is a directory\n", 2);
                shell->exit_status = 126;
                return (free(pathname), NULL);
            }
            if (access(pathname, X_OK) == 0)
                return (pathname);
            perror(cmd);
            shell->exit_status = 126;
            return (free(pathname), NULL);
        }
        free(pathname);
        i++;
    }
    shell->exit_status = 127;
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": command not found\n",2);
    return (NULL);
}