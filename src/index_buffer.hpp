#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include <GLFW/glfw3.h>

class IndexBuffer
{

private:
	unsigned int id;
	unsigned int count;

public:
	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();

	void bind();
	void unbind();
	void draw();
};

#endif