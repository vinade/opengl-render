all:
	g++ -std=c++0x -o bin/depth src/main.cpp -lGL -lGLU -lglut -lGLEW -lglfw
