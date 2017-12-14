INCLUDES	+= filler
INCLUDES	:= $(addprefix src/, $(INCLUDES))
INCLUDES	:= $(addsuffix .o, $(INCLUDES))
CC			:= gcc
FLAGS		+= -Wall -Wextra -Werror -Ilibft/includes/ -Iincludes/
FLAGS		+= -g
NAME		:= res/players/bmontoya.filler

.PHONY: clean fclean re all

all: $(NAME)

$(NAME): $(INCLUDES) libft/libft.a
	@echo 'Building $(NAME)'
	@$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	@$(CC) $(FLAGS) -c $^ -o $@

libft/libft.a:
	@make -C libft/

clean:
	@echo 'Removing $(NAME)--object files'
	@make -C libft/ clean
	@rm -rf $(INCLUDES)

fclean: clean
	@echo 'Removing $(NAME)'
	@make -C libft/ fclean
	@rm -rf $(NAME)

re: fclean all
