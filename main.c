#include "minishell.h"


int main(int argc, char **argv, char **envp)
{
    t_shell shell;
	char	*line;
	char	*prompt;
	t_token	*tokens;
	char	**paths;

	(void)argv;
	if (argc != 1) return (1);
	prompt = "minishell$ ";
	if (envp[0] == NULL)
		shell.env = create_minimall_envp();
    else
		shell.env = init_env(envp);
	paths = get_paths(shell.env);
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
		//free_arr(paths); it is need to be developed
		free_envs(shell.env);
		free_tokens(tokens);
	}
	
    return (0);
}