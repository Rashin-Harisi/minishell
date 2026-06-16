#include "minishell.h"

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