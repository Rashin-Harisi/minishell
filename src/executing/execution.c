#include "minishell.h"

int special_command_check(t_cmd *cmd, t_shell *shell)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);

    if (ft_strncmp(cmd->args[0], ".", 2) == 0)
    {
        ft_putstr_fd(".: command not found\n", 2);
        shell->exit_status = 127;
        return (1);
    }
    if (ft_strncmp(cmd->args[0], "..", 3) == 0)
    {
        ft_putstr_fd("..: command not found\n", 2);
        shell->exit_status = 127;
        return (1);
    }
    return (0);
}

int special_command_check_all(t_cmd *cmds, t_shell *shell)
{
    while (cmds)
    {
        if (special_command_check(cmds, shell))
            return (1);
        cmds = cmds->next;
    }
    return (0);
}

int execution(t_shell *shell, t_token **tokens, char **paths)
{
    if (special_command_check(shell->cmds, shell))
        return (shell->exit_status);
    if (check_redirection(shell->cmds))
        return (1);
    if (count_node(shell->cmds) == 1)
        return execute_single_command(shell, paths, tokens);
    else
        return execute_pipeline(shell, paths, tokens);
    return (0);
}