all:
	g++ -std=c++0x -o bin/depth src/main.cpp  src/3rd/imgui/*.cpp -lGL -lGLU -lglut -lGLEW
