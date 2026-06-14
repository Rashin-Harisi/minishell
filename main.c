#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char *line;
	char *prompt;
	t_token *tokens;
	char **paths;
	char *msg;

	(void)argv;
	if (argc != 1)
		return (1);
	ft_memset(&shell, 0, sizeof(t_shell));
	if (envp[0] == NULL)
		shell.env = create_minimal_envp();
	else
		shell.env = init_env(envp);
	paths = get_paths(shell.env);
	// print_envs(shell.env);
	// print_paths(paths);
	while (1)
	{
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
		if (*line)
			add_history(line);
		tokens = create_tokens(line);
		if (!tokens && !is_empty_line(line))
		{
			printf("Syntax error quotation\n");
			shell.exit_status = 1;
			free(line);
			rl_on_new_line();
			continue;
		}
		if (syntax_check(tokens))
		{
			printf("Syntax error pipe, redirection, semicolon, ampersand, or parenthesis\n");
			shell.exit_status = 1;
			free_tokens(tokens);
			free(line);
			rl_on_new_line();
			continue;
		}
		shell.cmds = create_cmds(tokens);
		if (!shell.cmds)
		{
			printf("cmds creation fail\n");
			free_tokens(tokens);
			free(line);
			continue;
		}
		expansion(&shell);
		heredoc_preparation(shell.cmds);
		builtin_functions(&shell, &tokens);
		msg = NULL;
		if (execution(&shell, &tokens, paths, &msg))
		{
			if (msg)
			{
				printf("minishell: exec: %s\n", msg);
				free(msg);
			}
			shell.exit_status = 1;
			free_tokens(tokens);
			free_cmds(shell.cmds);
			free(line);
			continue;
		}
		// print_tokens(tokens);
		// print_cmds(shell.cmds);
		free_cmds(shell.cmds);
		free_tokens(tokens);
		free(line);
	}
	free_paths(paths);
	free_envs(shell.env);
	return (0);
}