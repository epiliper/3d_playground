editor: main.c window.o keybindings.o
	gcc -o editor main.c window.o keybindings.o $(shell pkg-config --libs glfw3 cglm)

window.o: window.h window.c keybindings.o
	gcc -c window.h window.c keybindings.o

keybindings.o: keybindings.h keybindings.c
	gcc -c keybindings.h keybindings.c
