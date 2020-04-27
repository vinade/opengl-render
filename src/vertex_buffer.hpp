#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>


class VertexBuffer {

private:
	unsigned int id;

public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(float* data, unsigned int count);
	~VertexBuffer();

	void bind();
	void unbind();
};

#endif