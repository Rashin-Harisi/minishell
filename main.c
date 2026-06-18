#include "minishell.h"

static void free_iteration(t_token *tokens, t_cmd *cmds, char *line)
{
	if (cmds) free_cmds(cmds);
	if (tokens) free_tokens(tokens);
	if (line) free(line);
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char *line;
	char *prompt;
	t_token *tokens;
	char **paths;

	(void)argv;
	if (argc != 1)
		return (1);
	ft_memset(&shell, 0, sizeof(t_shell));
	if (envp[0] == NULL)
		shell.env = create_minimal_envp();
	else
		shell.env = init_env(envp);
	paths = NULL;
	//print_envs(shell.env);
	//print_paths(paths);
	while (1)
	{
		tokens = NULL;
		shell.cmds = NULL;
		init_signals();
		prompt = create_prompt(&shell);
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if (is_empty_line(line))
		{
			free(line);
			continue;
		}
		add_history(line);
		tokens = create_tokens(line);
		if (!tokens)
		{
			printf("Syntax error quotation\n");
			shell.exit_status = 1;
			free_iteration(tokens, shell.cmds, line);
			rl_on_new_line();
			continue;
		}
		//print_tokens(tokens);
		if (syntax_check(tokens))
		{
			printf("Syntax error pipe, redirection, semicolon, ampersand, or parenthesis\n");
			shell.exit_status = 1;
			free_iteration(tokens, shell.cmds, line);
			rl_on_new_line();
			continue;
		}
		shell.cmds = create_cmds(tokens);
		if (!shell.cmds)
		{
			printf("cmds creation fail\n");
			shell.exit_status = 1;
			free_iteration(tokens, shell.cmds, line);
			continue;
		}
		if (expansion(&shell))
		{
			shell.exit_status = 1;
			free_iteration(tokens, shell.cmds, line);
			continue;
		}
		heredoc_preparation(shell.cmds);
		free_paths(paths);
		paths = get_paths(shell.env);
		if (execution(&shell, &tokens, paths))
			shell.exit_status = 1;
		free_iteration(tokens, shell.cmds, line);
	}
	free_paths(paths);
	free_envs(shell.env);
	rl_clear_history();
	return (shell.exit_status);
}