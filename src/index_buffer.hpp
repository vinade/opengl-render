#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>


class IndexBuffer {

private:
	unsigned int id;
	unsigned int count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind();
	void unbind();
	void draw();
};

#endif