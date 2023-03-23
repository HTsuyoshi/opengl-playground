CC       = g++
NAME     = shaders_ex_01
FLAGS    = -g
INCLUDES = -I ../lib
GLAD     = ../../lib/GLAD/src/glad.c

.PHONY: 'build'
build: $(NAME).cpp
	@echo 'Building...'
	$(CC) -o $(NAME) $< $(FLAGS) $(GLAD) $(INCLUDES) `pkg-config --static --libs glfw3`

.PHONY: 'start'
start: build
	@echo 'Executing...'
	./$(NAME)

.PHONY: 'clean'
clean:
	@echo 'Removing...'
	rm $(NAME)
