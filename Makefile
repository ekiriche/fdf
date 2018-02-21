NAME = fdf

$(NAME) : all

all:
	gcc -g -I /usr/local/include main.c -L /usr/local/lib -lmlx -framework OpenGL -framework AppKit -o $(NAME) libftprintf.a

clean:
	rm -f .*o

fclean : clean
	rm -f $(NAME)

re: fclean all
