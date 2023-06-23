CC		=	g++
NAME	=	paint
SRC		=	button.cc \
			example.cc \
			GUI.cc \
			window.cc \

OBJ		=	$(SRC:.cc=.o)
HEADER	=	button.h GUI.h window.h aux.h

%.o: %.cc $(HEADER)
	$(CC) $< -c -o $@ `pkg-config --cflags --libs gtk+-3.0`

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $@ `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: clean fclean all