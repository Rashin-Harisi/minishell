#include "minishell.h"

char *check_access_pathname(char **paths, char *cmd, t_shell *shell)
{
    int i;
    char *temp;
    char *pathname;

    i = 0;
    if (!cmd || !cmd[0]) return (NULL);
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        shell->exit_status = 126;
        perror(cmd);
        return (NULL);
    }
    if (!paths)
    {
        shell->exit_status = 127;
        ft_putstr_fd("minishell: ",2);
        ft_putstr_fd(cmd ,2);
        ft_putstr_fd(": command not found\n",2);
        return (NULL);
    }
    while (paths[i])
    {
        temp = ft_strjoin(paths[i], "/");
        if (!temp) return (NULL);
        pathname = ft_strjoin(temp, cmd);
        free(temp);
        if (!pathname) return (NULL);
        if (access(pathname, X_OK) == 0)
            return (pathname);
        free(pathname);
        i++;
    }
    shell->exit_status = 127;
    ft_putstr_fd("minishell: ",2);
    ft_putstr_fd(cmd ,2);
    ft_putstr_fd(": command not found\n",2);
    return (NULL);
}