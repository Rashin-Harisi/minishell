#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
//# include <sys/acl.h>
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
# include "libft/libft.h"
/*======================Forward Declaration=============*/
typedef struct s_env t_env;
typedef struct s_cmd t_cmd;
typedef enum e_token_type t_token_type;
typedef struct s_token t_token;
typedef enum e_redir_type t_redir_type;
typedef struct s_redir t_redir;

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
    int     exit_status;
    int     interactive; // terminal or script =>isatty(STDIN_FILEN)
} t_shell;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
} t_env; //linked list for each key-value pair of env list

typedef struct s_token
{
    char            *value;
    t_token_type   type;
    struct s_token  *next;
} t_token;

typedef struct s_redir
{
    t_redir_type     type;
    char            *filename;
    struct s_redir  *next;
} t_redir;

typedef struct s_cmd
{
    char            **args;
    t_redir         *redirects;
    struct s_cmd    *next;
} t_cmd;

/*=================functions====================*/
void    extract_key_value(char *str,char **key, char **value);
void	ft_lstadd_back_env(t_env **env, t_env *new);
t_env   *init_env(char **envp);
void    init_cmds(char *line);

#endif
