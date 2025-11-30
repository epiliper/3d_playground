BUILD_DIR := build
CFLAGS := -Ithirdparty -g3 -g -lm
GL := thirdparty/glad.c

MAKE_BUILD_DIR := $(shell mkdir -p $(BUILD_DIR))

editor: main.c window dummy renderable utils state mouse state.c camera keybindings utils glad app
	gcc -o editor main.c $(BUILD_DIR)/window.o $(BUILD_DIR)/keybindings.o \
		state.c $(BUILD_DIR)/renderable.o $(BUILD_DIR)/mouse.o $(BUILD_DIR)/dummy.o \
		$(BUILD_DIR)/camera.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/glad.o $(BUILD_DIR)/app.o \
		$(shell pkg-config --libs glfw3 cglm) $(CFLAGS);
	./editor

renderable: renderable.h renderable.c
	gcc -c renderable.c -o $(BUILD_DIR)/renderable.o $(CFLAGS)

window: window.h window.c keybindings
	gcc -c window.c $(BUILD_DIR)/keybindings.o -o $(BUILD_DIR)/window.o $(CFLAGS)

keybindings: keybindings.h keybindings.c
	gcc -c keybindings.c -o $(BUILD_DIR)/keybindings.o $(CLAGS)

utils: utils.c utils.h
	gcc -c utils.c -o $(BUILD_DIR)/utils.o $(CFLAGS)

app: app.c app.h window.c window.h
	gcc -c app.c -o $(BUILD_DIR)/app.o $(CFLAGS)

camera: camera.h camera.c
	gcc -c camera.c -o $(BUILD_DIR)/camera.o $(CFLAGS)

state: state.c state.h
	gcc -c state.c -o $(BUILD_DIR)/state.o $(CFLAGS)

dummy: dummy.c dummy.h
	gcc -c dummy.c -o $(BUILD_DIR)/dummy.o $(CFLAGS)

mouse: mouse.c mouse.h window.h window.c
	gcc -c mouse.c -o $(BUILD_DIR)/mouse.o $(CFLAGS)

glad: glad.c
	gcc -c glad.c -o $(BUILD_DIR)/glad.o $(CFLAGS)
