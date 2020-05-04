NAME = depth
BUILD_FOLDER = ./bin
DEST = $(BUILD_FOLDER)/$(NAME)
CC = g++ -std=c++11
CFLAGS = -g -Wall -Wextra
LIBS = -lGL -lGLEW -lglfw -lassimp

VENDOR_SRC_FILES = stb_image.cpp \
	imgui_widgets.cpp \
	imgui_draw.cpp \
	imgui_impl_opengl3.cpp \
	imgui_impl_glfw.cpp \
	imgui.cpp

SRC_FILES  = $(VENDOR_SRC_FILES) \
	imgui_controller.cpp \
	vertex_buffer_layout.cpp \
	vertex_buffer.cpp \
	index_buffer.cpp \
	vertex_array.cpp \
	camera.cpp \
	perspective.cpp \
	texture.cpp \
	shader.cpp \
	mesh.cpp \
	debug_mesh.cpp \
	scene_item.cpp \
	light.cpp \
	render_window.cpp

OBJS = $(SRC_FILES:.cpp=.o)

SRC_FOLDER = ./src
VENDOR_FOLDER = $(SRC_FOLDER)/vendor
STB_FOLDER = $(VENDOR_FOLDER)/stb
IMGUI_FOLDER = $(VENDOR_FOLDER)/imgui

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(DEST) src/main.cpp $(LIBS)

.SECONDEXPANSION:
%.o: $$(wildcard $(SRC_FOLDER)/$$*.cpp) $$(wildcard $(STB_FOLDER)/$$*.cpp) $$(wildcard $(IMGUI_FOLDER)/$$*.cpp)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	rm *.o
