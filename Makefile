RED     = \033[31m
GREEN   = \033[32m
MAGENTA = \033[35m
CYAN    = \033[36m
RESET   = \033[0m

NAME		= minishell
HEADER      = minishell.h libft/libft.h

MANDATORY_SRC	= main.c\
				./env/env.c \
				./env/create_minimal_envp.c \
				./env/get_path.c \
				./env/helper.c \
				./signals/signals.c \
				./utils/utils.c \
				./utils/prompt.c \
				./utils/get_next_line.c\
				./utils/main_utils_first.c\
				./utils/main_utils_second.c\
				./src/parsing/tokens.c \
				./src/parsing/tokens_utils_first.c\
				./src/parsing/tokens_utils_second.c\
				./src/parsing/tokens_utils_third.c\
				./src/parsing/syntax_check.c \
				./src/parsing/syntax_utils.c \
				./src/parsing/cmds.c \
				./src/parsing/cmds_utils_first.c\
				./src/parsing/cmds_utils_second.c\
				./src/parsing/cmds_utils_third.c\
				./src/parsing/cmds_utils_forth.c\
				./src/parsing/expansion.c \
				./src/parsing/expansion_utils_first.c\
				./src/parsing/expansion_utils_second.c\
				./src/parsing/expansion_utils_third.c\
				./src/heardoc_prepration/prepration.c \
				./src/heardoc_prepration/utils.c \
				./src/builtins/update_env.c \
				./src/builtins/builtin_funcs.c \
				./src/builtins/cd.c\
				./src/builtins/cd_utils.c\
				./src/builtins/echo.c\
				./src/builtins/env.c\
				./src/builtins/exit.c\
				./src/builtins/export.c\
				./src/builtins/export_utils.c\
				./src/builtins/helpers.c\
				./src/builtins/pwd.c\
				./src/builtins/unset.c\
				./src/executing/execution.c \
				./src/executing/access.c \
				./src/executing/access_utils.c \
				./src/executing/pipeline_command.c \
				./src/executing/pipeline_utils_first.c \
				./src/executing/pipeline_utils_second.c \
				./src/executing/pipeline_utils_third.c \
				./src/executing/pipeline_utils_forth.c \
				./src/executing/redirections.c \
				./src/executing/redirection_utils.c \
				./src/executing/single_command.c \
				./src/executing/single_utils_first.c \
				./src/executing/single_utils_second.c \
				./src/executing/utils.c \
				./src/heardoc_prepration/utils_second.c

MANDATORY_OBJ	= $(MANDATORY_SRC:.c=.o)

LIBFT_PATH	= ./libft
LIBFT		= libft.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g -I.

all: $(NAME)

$(NAME): $(LIBFT) $(MANDATORY_OBJ)
	@printf "$(GREEN)Linking $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) $(MANDATORY_OBJ) $(LIBFT) -lreadline -o $(NAME)
	@printf "$(GREEN)Mandatory part is built.$(RESET)\n"

$(LIBFT):
	@printf "$(CYAN)Building libft...$(RESET)\n"
	@$(MAKE) --no-print-directory -s -C $(LIBFT_PATH)
	@cp $(LIBFT_PATH)/libft.a .

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "$(RED)Removing object files... $(RESET)\n"
	@rm -f $(MANDATORY_OBJ)
	@$(MAKE) clean --no-print-directory -s -C $(LIBFT_PATH)

fclean: clean
	@printf "$(MAGENTA)Removing $(NAME) and $(LIBFT)... $(RESET)\n"
	@rm -f $(NAME) libft.a libftprintf.a
	@$(MAKE) fclean --no-print-directory -s -C $(LIBFT_PATH)

re: fclean all

.PHONY: all bonus clean fclean re