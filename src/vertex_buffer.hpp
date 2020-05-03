#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <GLFW/glfw3.h>

class VertexBuffer
{

private:
	unsigned int id;

public:
	VertexBuffer(const void *data, unsigned int size);
	VertexBuffer(float *data, unsigned int count);

	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

#endif