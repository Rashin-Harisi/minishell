#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <fcntl.h> // for open()/ file flags
# include <sys/wait.h>
# include <sys/stat.h>// stat/lstat/fstat
# include <errno.h>
# include <termcap.h>
# include <linux/limits.h>
# include "libft/libft.h"

/*======================Forward Declaration=============*/
typedef struct s_env t_env;
typedef struct s_cmd t_cmd;
typedef enum e_token_type t_token_type;
typedef struct s_token t_token;
typedef enum e_redir_type t_redir_type;
typedef struct s_redir t_redir;

/*===================== marcos for custome terminal============*/
# define GREEN "\001\033[32m\002"
# define RED "\001\033[31m\002"
# define CYAN "\001\033[36m\002"
# define YELLOW  "\001\033[33m\002"
# define BLUE    "\001\033[34m\002"
# define RESET "\001\033[0m\002"

/*================Enums=================*/

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC
} t_token_type; // different types of token that might given to in the terminal

typedef enum e_redir_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redir_type;

/*===================Structs====================*/

typedef struct s_shell
{
    t_env   *env;
    t_cmd   *cmds;
    char    *line;
    char    *syntax_check;
    int     in_pipe;
    int     exit_status;
    int     interactive; // terminal or script =>isatty(STDIN_FILEN)
} t_shell;

typedef struct s_env
{
    char            *key;
    char            *value;
    int             has_equal;
    struct s_env    *next;
} t_env; //linked list for each key-value pair of env list

typedef struct s_token
{
    char            *value;
    t_token_type   type;
    int            quoted;
    struct s_token  *next;
} t_token;

typedef struct s_redir
{
    t_redir_type     type;
    int              fd;
    char            *filename;
    int             quoted;
    struct s_redir  *next;
} t_redir;

typedef struct s_cmd
{
    char            **args;
    t_redir         *redirects;
    struct s_cmd    *next;
} t_cmd;

typedef struct s_flags
{
    int single_quote;
    int double_quote;
} t_flags;

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

/*=================functions====================*/
// ENV's functions
int     extract_key_value(char *str,char **key, char **value);
void	ft_lstadd_back_env(t_env **env, t_env *new);
void    free_envs(t_env *env);
t_env   *init_env(char **envp);
t_env   *create_minimal_envp(void);
char    *get_env_value(t_env *env, char *key);
t_env   *create_env_node(char *key, char *value);
t_env   *find_node(t_env *env, char *key);
// PATH's function
char	**get_paths(t_env *env);
void	free_paths(char **paths);
// PROMPT's functions
char    *create_prompt(t_shell *shell);
// SIGNAL's functions
void    init_signals(void);
// TOKENS' functions
void    free_tokens(t_token *tokens);
void	ft_lstadd_back_token(t_token **token, t_token *new);
t_token *init_token(char *tmp, t_token *node);
char    *each_part_extract(char *line, int *index, int *syntax_error);
void    skip_spaces(char *line, int *index);
t_token *create_tokens(char *line);
// SYNTAX_CHECK's functions
int syntax_check(t_token *tokens, t_shell *shell);
// CMDS' functions
t_cmd   *create_cmds(t_token *tokens);
void    free_cmds(t_cmd *cmds);
void    free_array(char **args);
void    free_redirects(t_redir *redir);
void    free_one_cmd(t_cmd *cmd);
void    ft_lstadd_back_cmds(t_cmd **cmds, t_cmd *node);
void    ft_lstadd_back_redirects(t_redir **redir, t_redir *node);
t_redir *init_redirect(t_token *tokens);
int     count_args(t_token *tokens);
// EXPANSION's functions
int     expansion(t_shell *shell);
int     is_specific_char(char s);
char    *find_var_value(char *var, t_env *envs);
char    *append_str(char *str, char *s);
char    *append_char(char *str, char s);
char    *expand_var(char *str, int *i, t_shell *shell, char *expanded);
char    *expansion_string(char *str, t_shell *shell);
int     expansion_args(char **args, t_shell *shell);
int     expansion_redir(t_redir *redir, t_shell *shell);
int expansion_tokens(t_token *tokens, t_shell *shell);
// HEARDOC_PREPRARTION's functions
void    heredoc_preparation(t_cmd *cmds, t_shell *shell);
// BUILTIN's functions 
int     builtin_functions(t_shell *shell, t_token **tokens);
int     builtin_update_env(t_shell *shell, t_cmd *cmd);
int     pwd_func(t_shell *shell);
int     echo_func(t_shell *shell, t_cmd *cmd);
int     exit_func(t_shell *shell, t_token *tokens, t_cmd *cmd);
int     cd_env_func(char **args, t_shell *shell);
int     env_func(t_shell *shell);
int     export_env_func(char **args, t_shell *shell);
t_env   *find_node(t_env *env, char *key);
int     update_env_value(t_env **env, char *key, char *new_value);
int     is_valid_name(char *str);
void    print_identifier_error(char *func, char *str, char *value);
int     unset_env_func(char **args, t_shell *shell);

// UTILS' functions
void	print_envs(t_env *env);
void	print_tokens(t_token *tokens);
void	print_paths(char **paths);
int	    is_empty_line(char *line);
void	print_cmds(t_cmd *cmds);
void    free_array(char **args);
char    *get_next_line(int fd);
//EXECUTION's functions
int     execution(t_shell *shell, t_token **tokens, char **paths);
int     check_redirection(t_cmd *cmds);
int     only_redirection(t_cmd *cmd, t_shell *shell);
int     apply_redirection(t_redir *redirects);
int     count_node(t_cmd *cmds);
int     is_builtin(char **args);
int     calculate_nodes(t_env *env);
char    **convert_list_to_array(t_env *env);
char    *check_access_pathname(char **paths, char *cmd, t_shell *shell);
int     execute_external_command(t_cmd *cmd, char **paths, t_shell *shell);
int     builtin_with_redirection(t_shell *shell, t_token **tokens);
int     execute_single_command(t_shell *shell, char **paths, t_token **tokens);
int     execute_pipeline(t_shell *shell, char **paths, t_token **tokens);

#endif
