NAME		= minishell
HEADER      = minishell.h libft/libft.h

MANDATORY_SRC	= main.c\
				env.c \
				cmds.c

MANDATORY_OBJ	= $(MANDATORY_SRC:.c=.o)

LIBFT_PATH	= ./libft
LIBFT		= libft.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(LIBFT) $(MANDATORY_OBJ)
	$(CC) $(CFLAGS) $(MANDATORY_OBJ) $(LIBFT) -o $(NAME)
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