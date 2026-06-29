#include "minishell.h"

int cd_env_func(char **args, t_shell *shell)
{
    char *path;
    char *oldpwd_env;
    char old_pwd[PATH_MAX];
    char curr_pwd[PATH_MAX];

    if (args[1] && args[2])
    {
        ft_putstr_fd("cd : there are too many arguments\n", 2);
        shell->exit_status = 1;
        return (1);
    }
    if (!getcwd(old_pwd, sizeof(old_pwd)))
        return (perror("getcwd"), shell->exit_status = 1, 1);
    if (!args[1])
    {
        path = get_env_value(shell->env, "HOME");
        if (!path)
        {
            ft_putstr_fd("cd: Home not set\n", 2);
            shell->exit_status = 1;
            return (1);
        }
    }
    else if (ft_strncmp(args[1],"-",2) == 0)
    {
        oldpwd_env = get_env_value(shell->env, "OLDPWD");
        if (!oldpwd_env)
        {
            ft_putstr_fd("cd: OLDWPD not set\n",2);
            shell->exit_status = 1;
            return (1);
        }
        path = oldpwd_env;
        ft_putstr_fd(path, 1);
        ft_putstr_fd("\n", 1);
    }
    else
        path = args[1];
    if (chdir(path) == -1)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": ", 2);
        perror("");
        shell->exit_status = 1;
        return (1);
    }
    if (!getcwd(old_pwd, sizeof(old_pwd)))
        return (perror("getcwd"), shell->exit_status = 1, 1);
    if (update_env_value(&shell->env, "OLDPWD", old_pwd))
        return (shell->exit_status = 1, 1);
    if (update_env_value(&shell->env, "PWD", curr_pwd))
        return (shell->exit_status = 1, 1);
    shell->exit_status = 0;
    return (0);
}