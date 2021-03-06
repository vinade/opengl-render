#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

class VertexArray
{

private:
	unsigned int id;

public:
	VertexArray();
	~VertexArray();

	void add_buffer(VertexBuffer *vbo, VertexBufferLayout *layout);
	void bind();
	void unbind();
};

#endif