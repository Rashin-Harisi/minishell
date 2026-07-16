#include "minishell.h"

static void free_iteration(t_token *tokens, t_cmd *cmds, char *line)
{
	if (cmds) free_cmds(cmds);
	if (tokens) free_tokens(tokens);
	if (line) free(line);
}

void remove_empty_args(char **args)
{
    int i;
    int j;

    if (!args)
        return ;
    i = 0;
    j = 0;
    while (args[i])
    {
        if (args[i][0] != '\0')
        {
            args[j] = args[i];
            j++;
        }
        else
            free(args[i]);
        i++;
    }
    args[j] = NULL;
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char *prompt;
	t_token *tokens;
	char **paths;
	int	ret; // 0: command is executed, continue; 1 : error, continue, 2: exit command, shell is done

	(void)argv;
	if (argc != 1)
		return (1);
	ft_memset(&shell, 0, sizeof(t_shell));
	if (envp[0] == NULL)
		shell.env = create_minimal_envp();
	else
		shell.env = init_env(envp);
	paths = NULL;
	shell.interactive = isatty(STDIN_FILENO);
	//print_envs(shell.env);
	//print_paths(paths);
	while (1)
	{
		tokens = NULL;
		shell.cmds = NULL;
		set_prompt_signals();
		if (shell.interactive)
		{
			prompt = create_prompt(&shell);
			shell.line = readline(prompt);
			free(prompt);
		}
		else
		{
			shell.line = get_next_line(STDIN_FILENO);
			if (shell.line && ft_strlen(shell.line) > 0
    			&& shell.line[ft_strlen(shell.line) - 1] == '\n')
    			shell.line[ft_strlen(shell.line) - 1] = '\0';
		}
		if (!shell.line)
		{
			if (shell.interactive)
				ft_putstr_fd("exit\n", STDERR_FILENO);
			break;
		}
		if (is_empty_line(shell.line))
		{
			free(shell.line);
			continue;
		}
		if (shell.interactive) add_history(shell.line);
		tokens = create_tokens(shell.line);
		if (!tokens)
		{
			ft_putstr_fd("Syntax error quotation\n", STDERR_FILENO);
			shell.exit_status = 2;
			free_iteration(tokens, shell.cmds, shell.line);
			rl_on_new_line();
			continue;
		}
		//print_tokens(tokens);
		if (syntax_check(tokens, &shell))
		{
			ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
			ft_putstr_fd(shell.syntax_check, STDERR_FILENO);
			ft_putstr_fd("'\n", STDERR_FILENO);
			shell.exit_status = 2;
			free_iteration(tokens, shell.cmds, shell.line);
			rl_on_new_line();
			continue;
		}
		if (expansion_tokens(tokens, &shell))
		{
			shell.exit_status = 1;
			free_iteration(tokens, shell.cmds, shell.line);
			continue;
		}
		shell.cmds = create_cmds(tokens);
		if (!shell.cmds)
		{
			ft_putstr_fd("cmds creation fail\n", STDERR_FILENO);
			shell.exit_status = 1;
			free_iteration(tokens, shell.cmds, shell.line);
			continue;
		}
		
		if (expansion(&shell))
		{
			shell.exit_status = 1;
			free_iteration(tokens, shell.cmds, shell.line);
			continue;
		}
		//remove_empty_args(shell.cmds->args);
		//print_cmds(shell.cmds);
		heredoc_preparation(shell.cmds, &shell);
		free_paths(paths);
		paths = get_paths(shell.env);
		ret = execution(&shell, &tokens, paths);
		free_iteration(tokens, shell.cmds, shell.line);
		if (ret == 2) break;
		if (ret == 1) continue;
	}
	if (paths)
		free_paths(paths);
	free_envs(shell.env);
	rl_clear_history();
	//clear_history();
	//rl_free_line_state();
	//rl_cleanup_after_signal();
	return (shell.exit_status);
}