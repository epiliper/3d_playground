BUILD_DIR := build
CFLAGS := -Ithirdparty -g3 -g -lm
GL := thirdparty/glad.c

MAKE_BUILD_DIR := $(shell mkdir -p $(BUILD_DIR))

editor: main.c window dummy renderable utils state mouse state.c camera keybindings utils glad app cube entity dynarray defs
	gcc -o editor main.c $(BUILD_DIR)/window.o $(BUILD_DIR)/keybindings.o \
		state.c $(BUILD_DIR)/renderable.o $(BUILD_DIR)/mouse.o $(BUILD_DIR)/dummy.o \
		$(BUILD_DIR)/camera.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/glad.o $(BUILD_DIR)/app.o $(BUILD_DIR)/cube.o \
		$(BUILD_DIR)/entity.o $(BUILD_DIR)/dynarray.o $(BUILD_DIR)/defs.o \
		$(shell pkg-config --libs glfw3 cglm) $(CFLAGS); ./editor

dynarray: dynarray.h dynarray.c
	gcc -c dynarray.c -o $(BUILD_DIR)/dynarray.o $(CFLAGS)

defs: defs.c defs.h
	gcc -c defs.c -o $(BUILD_DIR)/defs.o

renderable: renderable.h renderable.c
	gcc -c renderable.c -o $(BUILD_DIR)/renderable.o $(CFLAGS)

entity: entity.c entity.h
	gcc -c entity.c -o $(BUILD_DIR)/entity.o $(CFLAGS)

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

dummy: dummy.c dummy.h cube
	gcc -c dummy.c $(BUILD_DIR)/cube.o -o $(BUILD_DIR)/dummy.o $(CFLAGS)

mouse: mouse.c mouse.h window.h window.c
	gcc -c mouse.c -o $(BUILD_DIR)/mouse.o $(CFLAGS)

glad: glad.c
	gcc -c glad.c -o $(BUILD_DIR)/glad.o $(CFLAGS)

cube: cube.c cube.h
	gcc -c cube.c -o $(BUILD_DIR)/cube.o $(CFLAGS)
