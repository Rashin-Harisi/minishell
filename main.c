#include "minishell.h"


int main(int argc, char **argv, char **envp)
{
    t_shell shell;
	char	*line;
	char	*prompt;
	t_token	*tokens;

	(void)argc;
	(void)argv;
	prompt = "minishell$ ";
    shell.env = init_env(envp);
	while (1)
	{
		line = readline(prompt);
		if (!line) break;
		if (*line) add_history(line);
		tokens = create_tokens(line);
		print_envs(shell.env);
		print_tokens(tokens);

		//shell.cmds = init_cmds(tokens);

		free(line);
		free_envs(shell.env);
		free_tokens(tokens);
	}
	
    return (0);
}