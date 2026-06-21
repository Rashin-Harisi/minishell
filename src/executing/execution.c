#include "minishell.h"

int execution(t_shell *shell, t_token **tokens, char **paths)
{
    if (check_redirection(shell->cmds))
        return (1);
    if (count_node(shell->cmds) == 1)
        return execute_single_command(shell, paths, tokens);
    else
        return execute_pipeline(shell, paths, tokens);
    return (0);
}