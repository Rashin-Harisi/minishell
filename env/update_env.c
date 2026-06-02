#include "minishell.h"

void    builtin_update_env(t_shell *shell)
{
    t_cmd   *cmds = shell->cmds;
    t_env   *env = shell->env;
    
    if(!cmds || !cmds->args || !cmds->args[0])
        return;
    while(cmds)
    {
        if (ft_strncmp(cmds->args[0], "cd", ft_strlen("cd") + 1) == 0)
            redirect_func(cmds->args[1], env);
        else if (ft_strncmp(cmds->args[0], "export", ft_strlen("export") + 1) == 0)
            exposrt_env_func(cmds->args[1], env);
        else if (ft_strncmp(cmds->args[0], "unset", ft_strlen("unset") + 1 ) == 0)
            unset_env_func(cmds->args[1], env);
        cmds = cmds->next;
    }
}