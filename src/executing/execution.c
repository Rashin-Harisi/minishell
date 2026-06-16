#include "minishell.h"

int execution(t_shell *shell, t_token **tokens, char **paths)
{
    if (check_redirection(shell->cmds))
        return (1);
    if (count_node(shell->cmds) == 1)
        execute_single_command(shell, paths, tokens);
    else
        execute_pipeline(shell, paths, tokens);
    return (0);
}