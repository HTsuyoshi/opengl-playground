CC    = g++
NAME  = hello_triangle
FLAGS = -g
GLAD  = ../../lib/GLAD/src/glad.c

build: $(NAME).cpp
	@echo 'Building...'
	$(CC) -o $(NAME) $< $(FLAGS) $(GLAD) `pkg-config --static --libs glfw3`

start: build
	@echo 'Executing...'
	./$(NAME)

clean:
	@echo 'Removing...'
	rm $(NAME)
