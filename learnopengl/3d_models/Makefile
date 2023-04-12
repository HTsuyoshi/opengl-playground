CC           = g++
NAME         = table
FLAGS        = -g
INCLUDES     = -Ilib -I../lib/imgui/include -I../lib/own/include -I../lib/GLAD/include
MYLIBS       = ../lib/own/src/shapes.cpp
GLAD         = ../lib/GLAD/src/glad.c
IMGUI_FOLDER = ../lib/imgui/src
IMGUI        = $(IMGUI_FOLDER)/imgui.cpp \
			   $(IMGUI_FOLDER)/imgui_impl_glfw.cpp \
			   $(IMGUI_FOLDER)/imgui_impl_opengl3.cpp \
			   $(IMGUI_FOLDER)/imgui_tables.cpp \
			   $(IMGUI_FOLDER)/imgui_widgets.cpp \
			   $(IMGUI_FOLDER)/imgui_draw.cpp \
			   $(IMGUI_FOLDER)/imgui_demo.cpp

.PHONY: 'build'
build: $(NAME).cpp
	@echo 'Building...'
	$(CC) -o $(NAME) $< $(FLAGS) $(MYLIBS) $(GLAD) $(IMGUI) $(INCLUDES) `pkg-config --static --libs glfw3`

.PHONY: 'start'
start: build
	@echo 'Executing...'
	./$(NAME)

.PHONY: 'clean'
clean:
	@echo 'Removing...'
	rm $(NAME)
