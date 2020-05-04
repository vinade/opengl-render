all:
	g++ -std=c++0x -g -Wall -Wextra -o bin/depth src/main.cpp  src/3rd/imgui/*.cpp -lGL -lGLEW -lglfw -lassimp
