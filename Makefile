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
				./src/parsing/tokens.c \
				./src/parsing/helpers_tokens.c\
				./src/parsing/syntax_check.c \
				./src/parsing/cmds.c \
				./src/parsing/helpers_cmds.c\
				./src/parsing/expansion.c \
				./src/parsing/helpers_expansion.c\
				./src/heardoc_prepration/prepration.c \
				./src/builtins/update_env.c \
				./src/builtins/builtin_funcs.c \
				./src/builtins/cd.c\
				./src/builtins/echo.c\
				./src/builtins/env.c\
				./src/builtins/exit.c\
				./src/builtins/export.c\
				./src/builtins/helpers.c\
				./src/builtins/pwd.c\
				./src/builtins/unset.c\
				./src/executing/execution.c \
				./src/executing/access.c \
				./src/executing/pipeline_command.c \
				./src/executing/redirections.c \
				./src/executing/single_command.c \
				./src/executing/utils.c 

MANDATORY_OBJ	= $(MANDATORY_SRC:.c=.o)

LIBFT_PATH	= ./libft
LIBFT		= libft.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g -I.

all: $(NAME)

$(NAME): $(LIBFT) $(MANDATORY_OBJ)
	$(CC) $(CFLAGS) $(MANDATORY_OBJ) $(LIBFT) -lreadline -o $(NAME)
	@echo "Mandatory part is built."

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)
	cp $(LIBFT_PATH)/libft.a .

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(MANDATORY_OBJ)
	$(MAKE) clean -C $(LIBFT_PATH)

fclean: clean
	rm -f $(NAME) libft.a libftprintf.a
	$(MAKE) fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all bonus clean fclean re