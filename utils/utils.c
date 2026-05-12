#include "minishell.h"

void	print_envs(t_env *env)
{
	printf("========== ENV ==========\n");
	while (env)
	{
		printf("KEY   : %s\n",
			env->key ? env->key : "(null)");

		printf("VALUE : %s\n",
			env->value ? env->value : "(null)");

		printf("-------------------------\n");

		env = env->next;
	}
}

char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("TOKEN_WORD");
	if (type == TOKEN_PIPE)
		return ("TOKEN_PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("TOKEN_REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("TOKEN_REDIR_OUT");
	if (type == TOKEN_APPEND)
		return ("TOKEN_APPEND");
	if (type == TOKEN_HEREDOC)
		return ("TOKEN_HEREDOC");
	return ("UNKNOWN");
}

void	print_tokens(t_token *tokens)
{
	printf("======== TOKENS =========\n");

	while (tokens)
	{
		printf("VALUE : [%s]\n",
			tokens->value ? tokens->value : "(null)");

		printf("TYPE  : %s\n",
			get_token_type_name(tokens->type));

		printf("-------------------------\n");

		tokens = tokens->next;
	}
}

void	print_paths(char **paths)
{
	int	i;

	i = 0;
	printf("========== PATHS ==========\n");
	if (!paths)
	{
		printf("(null)\n");
		return ;
	}
	while (paths[i])
	{
		printf("paths[%d] = [%s]\n", i, paths[i]);
		i++;
	}
}