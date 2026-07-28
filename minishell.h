/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabdolho <rabdolho@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 16:47:45 by rabdolho          #+#    #+#             */
/*   Updated: 2026/07/28 15:44:51 by rabdolho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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
# include <fcntl.h> // for open();/ file flags
# include <sys/wait.h>
# include <sys/stat.h>// stat/lstat/fstat
# include <errno.h>
# include <termcap.h>
# include <linux/limits.h>
# include "libft/libft.h"

/*===================Global Variable==================== */
extern volatile sig_atomic_t	g_signal;
/*======================Forward Declaration=============*/
typedef struct s_env			t_env;
typedef struct s_cmd			t_cmd;
typedef enum e_token_type		t_token_type;
typedef struct s_token			t_token;
typedef enum e_redir_type		t_redir_type;
typedef struct s_redir			t_redir;
typedef struct s_shell			t_shell;

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
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

/*===================Structs====================*/
typedef struct s_shell
{
	t_env	*env;
	t_cmd	*cmds;
	char	*line;
	char	*syntax_check;
	int		in_pipe;
	int		exit_status;
	int		heredoc_index;
	int		interactive;
}	t_shell;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				has_equal;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_redir_type	type;
	int				fd;
	char			*filename;
	int				quoted;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirects;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_flags
{
	int	single_quote;
	int	double_quote;
}	t_flags;

typedef struct s_main
{
	t_shell	shell;
	t_token	*tokens;
	char	**paths;
}	t_main;

typedef struct s_export
{
	t_env	*node;
	char	*key;
	char	*value;
	int		has_equal;
	int		status;
}	t_export;

typedef struct s_pipeline
{
	int		pipefd[2];
	int		prev_fd;
	int		status;
	int		count;
	int		index;
	pid_t	*pids;
	char	**envp;
	char	**paths;
	t_cmd	*cmd;
}	t_pipeline;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

/*=================functions====================*/
int				handle_line(t_main *data);
int				execute_commands(t_main *data);
int				shell_iteration(t_main *data);
void			cleanup_shell(t_main *data);
char			*get_next_line(int fd);;
void			free_iteration(t_token *tokens, t_cmd *cmds, char *line);
void			remove_empty_args(char **args);
void			init_main(t_main *data, char **envp);
char			*read_shell_line(t_shell *shell);
int				handle_token_error(t_main *data);
int				handle_syntax_error(t_main *data);
int				create_command_list(t_main *data);
int				prepare_commands(t_main *data);
char			*join_free(char *s1, char *s2);
char			*build_prompt(char *prompt, char *user, char *cwd);
char			*create_prompt(t_shell *shell);
char			*get_token_type_name(t_token_type type);
int				is_empty_line(char *line);
t_env			*create_env_node(char *key, char *value);
t_env			*create_minimal_envp(void);
int				extract_key_value(char *str, char **key, char **value);
void			ft_lstadd_back_env(t_env **env, t_env *new);
void			free_envs(t_env *env);
t_env			*create_env_from_string(char *str);
t_env			*init_env(char **envp);
void			free_paths(char **paths);
char			**get_paths(t_env *env);
char			*get_env_value(t_env *env, char *key);
void			set_prompt_signals(void);
void			set_parent_wait_signals(void);
void			set_child_signals(void);
int				builtin_functions(t_shell *shell, t_token **tokens);
int				cd_error(t_shell *shell, char *message);
int				get_old_pwd(t_shell *shell, char *old_pwd);
char			*get_cd_path(char **args, t_shell *shell);
int				change_directory(char *path, t_shell *shell);
int				update_cd_env(t_shell *shell, char *old_pwd, char *curr_pwd,
					int old_pwd_valid);
int				finish_cd(char **args, t_shell *shell, char *old_pwd,
					int old_pwd_valid);
int				cd_env_func(char **args, t_shell *shell);
int				is_echo_n_option(char *str);
int				echo_func(t_shell *shell, t_cmd *cmd);
int				env_func(t_shell *shell);
int				is_valid_arg(char *str);
void			print_exit_message(t_shell *shell);
int				handle_numeric_error(t_shell *shell, char *arg);
int				handle_too_many_args(t_shell *shell);
int				exit_func(t_shell *shell, t_token *tokens, t_cmd *cmd);
void			print_export(t_env *env);
void			free_export_data(t_export *data);
int				parse_export_arg(t_export *data, char *arg);
int				update_export_node(t_export *data);
int				add_export_node(t_export *data, t_shell *shell);
int				save_export_arg(t_export *data, t_shell *shell);
int				process_export_arg(char *arg, t_shell *shell, t_export *data);
int				export_env_func(char **args, t_shell *shell);
t_env			*find_node(t_env *env, char *key);
int				update_env_value(t_env **env, char *key, char *new_value);
int				is_valid_name(char *str);
void			print_identifier_error(char *func, char *str, char *value);
int				pwd_func(t_shell *shell);
int				remove_env_node(t_env **env, t_env *node);
int				unset_env_func(char **args, t_shell *shell);
int				builtin_update_env(t_shell *shell, t_cmd *cmd);
int				read_heredoc(int fd, t_redir *redir, t_shell *shell);
int				open_heredoc_file(char *filename);
int				finish_heredoc(t_redir *redir, int fd, char *filename);
int				prepare_reading_file(t_redir *redir, t_shell *shell);
int				heredoc_preparation(t_cmd *cmds, t_shell *shell);
char			*create_name(int index);
int				heredoc_error(int fd, char *filename);
char			*expand_heredoc_line(char *line, t_redir *redir,
					t_shell *shell);
int				write_heredoc_line(int fd, char *line);
int				is_directory(char *path);
void			print_error(char *s1, char *s2);
char			*check_direct_path(char *cmd, t_shell *shell);
char			*create_pathname(char *path, char *cmd, t_shell *shell);
char			*check_path_entry(char *path, char *cmd, t_shell *shell,
					int *found_not_executable);
void			print_command_error(char *cmd, t_shell *shell,
					int found_not_executable);
char			*search_paths(char **paths, char *cmd, t_shell *shell);
char			*check_access_pathname(char **paths, char *cmd, t_shell *shell);
int				special_command_check(t_cmd *cmd, t_shell *shell);
int				special_command_check_all(t_cmd *cmds, t_shell *shell);
int				execution(t_shell *shell, t_token **tokens, char **paths);
void			close_previous_fd(t_pipeline *data);
int				execute_pipeline(t_shell *shell, char **paths,
					t_token **tokens);
int				external_command_in_pipe(char **paths, t_cmd *cmd,
					t_shell *shell, char **envp);
int				pipe_pwd(t_shell *shell);
int				pipe_echo(t_shell *shell, t_cmd *cmd);
int				builtin_functions_in_pipe(t_shell *shell,
					t_token **tokens, t_cmd *cmd);
void			free_child_pipeline(t_shell *shell, t_pipeline *data,
					t_token **tokens);
void			update_parent_fds(t_pipeline *data);
int				create_child(t_shell *shell, t_pipeline *data,
					t_token **tokens);
int				run_pipeline(t_shell *shell, t_pipeline *data,
					t_token **tokens);
void			wait_pipeline(t_shell *shell, t_pipeline *data);
int				init_pipeline(t_shell *shell, char **paths, t_pipeline *data);
void			close_current_pipe(t_pipeline *data);
void			stop_pipeline_children(t_pipeline *data);
int				pipeline_error(t_shell *shell, t_pipeline *data,
					int pipe_created);
int				create_pipe(t_shell *shell, t_pipeline *data);
int				redirect_child_input(t_pipeline *data);
int				redirect_child_output(t_pipeline *data);
int				prepare_child_fds(t_pipeline *data);
void			exit_pipeline_child(t_shell *shell, t_pipeline *data,
					t_token **tokens, int status);
void			execute_child_builtin(t_shell *shell, t_pipeline *data,
					t_token **tokens);
void			execute_pipeline_child(t_shell *shell, t_pipeline *data,
					t_token **tokens);
int				open_redirection_file(t_redir *redir);
int				validate_redirection(t_redir *redir);
int				check_redirection(t_cmd *cmds);
int				save_standard_fds(int saved[2]);
int				restore_standard_fds(int saved[2]);
int				only_redirection(t_cmd *cmd, t_shell *shell);
int				get_redirection_fd(t_redir *redir);
int				duplicate_redirection_fd(t_redir *redir, int fd);
int				apply_single_redirection(t_redir *redir);
int				apply_redirection(t_redir *redirects);
int				handle_builtin_redirection(t_shell *shell, t_cmd *cmd,
					int saved[2]);
int				builtin_with_redirection(t_shell *shell, t_token **tokens);
int				execute_single_command(t_shell *shell, char **paths,
					t_token **tokens);
int				wait_for_pid(pid_t pid, int *status);
void			set_wait_status(t_shell *shell, int status);
void			free_external_child(t_shell *shell, char **paths,
					char **envp, char *pathname);
void			exit_external_child(t_shell *shell, char **paths,
					char *pathname, int status);
void			run_external_child(t_cmd *cmd, char **paths,
					t_shell *shell, char *pathname);
int				handle_fork_error(t_shell *shell, char *pathname);
int				wait_external_child(t_shell *shell, pid_t pid, char *pathname);
int				execute_external_command(t_cmd *cmd, char **paths,
					t_shell *shell);
int				restore_standard_single_fds(int saved_stdin, int saved_stdout);
int				count_node(t_cmd *cmds);
int				is_builtin(char **args);
int				calculate_nodes(t_env *env);
char			*create_env_line(t_env *env);
char			**convert_list_to_array(t_env *env);
int				set_redirect_type(t_redir *redir, t_token *tokens);
t_redir			*init_redirect(t_token *tokens);
int				is_redirection_type(t_token_type type);
int				is_valid_word(t_token *token);
int				count_args(t_token *tokens);
t_cmd			*create_cmd_node(int count);
int				is_redirection_token(t_token_type type);
int				add_word_to_cmd(t_cmd *cmd, t_token **tokens, int *index);
int				add_redirection_to_cmd(t_cmd *cmd, t_token **tokens);
int				fill_cmd(t_cmd *cmd, t_token **tokens);
void			free_array(char **args);
void			free_redirects(t_redir *redir);
void			free_one_cmd(t_cmd *cmd);
void			free_cmds(t_cmd *cmds);
void			ft_lstadd_back_cmds(t_cmd **cmds, t_cmd *node);
void			ft_lstadd_back_redirects(t_redir **redir, t_redir *node);
int				has_quote(char *str);
int				update_quote_cmd_state(char c, int *single, int *double_quote);
char			*removal_quote_only(char *str);
int				set_heredoc_redirect(t_redir *redir, t_token *tokens);
t_cmd			*init_cmds(t_token **tokens);
t_cmd			*create_cmds(t_token *tokens);
char			*append_next_character(char *str, int *i, char *expanded);
char			*expansion_string(char *str, t_shell *shell);
int				is_specific_char(char s);
char			*find_var_value(char *var, t_env *envs);
char			*append_str(char *str, char *s);
char			*append_char(char *str, char s);
char			*expand_exit_status(t_shell *shell, char *expanded);
char			*expand_environment_var(char *str, int *i,
					t_shell *shell, char *expanded);
char			*expand_var(char *str, int *i, t_shell *shell, char *expanded);
int				update_quote_state(char c, int *single_quote,
					int *double_quote);
char			*expand_next_part(char *str, int *i,
					t_shell *shell, char *expanded);
int				expansion_tokens(t_token *tokens, t_shell *shell);
int				expansion_args(char **args, t_shell *shell);
int				expansion_redir(t_redir *redir, t_shell *shell);
int				expansion(t_shell *shell);
int				set_redirection_error(t_token *curr, t_shell *shell);
int				validate_token(t_token *prev, t_token *curr, t_shell *shell);
int				syntax_check(t_token *tokens, t_shell *shell);
int				pipe_validation(t_token *prev, t_token *curr);
int				redir_validation(t_token *curr);
int				has_forbidden_chart(t_token *curr);
int				is_redirection(t_token_type type);
void			update_token_quotes(char c, t_flags *flags);
int				is_unquoted_operator(char *line, int index, t_flags *flags);
int				find_part_end(char *line, int *index, t_flags *flags);
char			*extract_word(char *line, int *index, int *syntax_error);
char			*each_part_extract(char *line, int *index, int *syntax_error);
int				is_shell_space(char c);
int				is_double_operator(char *line, int index);
int				is_single_operator(char c);
char			*extract_operator(char *line, int *index);
int				token_has_quote(char *value);
t_token_type	get_token_type(char *value);
t_token			*init_token(char *tmp, t_token *node);
void			ft_lstadd_back_token(t_token **token, t_token *new);
void			free_tokens(t_token *tokens);
t_token			*create_token_node(char *value);
int				add_token(t_token **tokens, char *value);
int				fill_tokens(char *line, t_token **tokens, int *syntax_error);
t_token			*create_tokens(char *line);
void			skip_spaces(char *line, int *index);

/*=================End========================= */
#endif
