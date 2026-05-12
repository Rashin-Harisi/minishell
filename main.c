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
	ft_memset(&shell, 0, sizeof(t_shell));
	if (envp[0] == NULL)
		shell.env = create_minimal_envp();
    else
		shell.env = init_env(envp);
	paths = get_paths(shell.env);
	//print_envs(shell.env);
	//print_paths(paths);
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
		if (*line) add_history(line);
		tokens = create_tokens(line);

		print_tokens(tokens);
		//shell.cmds = init_cmds(tokens);
		free(line);
		free_tokens(tokens);
	}
	free_paths(paths);
	free_envs(shell.env);
    return (0);
}