#include "minishell.h"

char	*join_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}

char	*create_prompt(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	*prompt;
	char	*user;

	if (shell->exit_status == 0)
		prompt = ft_strdup(GREEN "➜ " RESET);
	else
		prompt = ft_strdup(RED "➜ " RESET);
	if (!prompt)
		return (NULL);
	user = get_env_value(shell->env, "USER");
	if (!user)
		user = "minishell";
	if (!getcwd(cwd, sizeof(cwd)))
		return (join_free(prompt, CYAN "minishell" RESET " $ "));
	prompt = join_free(prompt, CYAN);
	prompt = join_free(prompt, user);
	prompt = join_free(prompt, RESET " ~");
	prompt = join_free(prompt, BLUE);
	prompt = join_free(prompt, cwd);
	prompt = join_free(prompt, RESET " $ ");
	return (prompt);
}