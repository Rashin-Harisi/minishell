#include "minishell.h"


int main(int argc, char **argv, char **envp)
{
    t_shell shell;
	char	*line;
	char	*prompt;

	(void)argc;
	(void)argv;
	prompt = "minishell$ ";
    shell.env = init_env(envp);
	while (1)
	{
		line = readline(prompt);
		if (!line) break;
		if (*line) add_history(line);
		shell.cmds = init_cmds(line);
		free(line);
	}
	
    return (0);
}