#ifndef VERTEX_BUFFER_LAYOUT_CPP
#define VERTEX_BUFFER_LAYOUT_CPP

#include "vertex_buffer_layout.hpp"

void VertexBufferLayout::push(DataType type, unsigned int count)
{
	unsigned int gl_type = GL_FLOAT;
	unsigned char normalized = GL_FALSE;

	switch (type)
	{
	case DATA_TYPE_UINT:
		gl_type = GL_UNSIGNED_INT;
		normalized = GL_FALSE;
		break;
	case DATA_TYPE_BYTE:
		gl_type = GL_UNSIGNED_BYTE;
		normalized = GL_TRUE;
		break;
	case DATA_TYPE_FLOAT:
	default:
		gl_type = GL_FLOAT;
		normalized = GL_FALSE;
		break;
	}

	this->elements.push_back({gl_type, count, normalized});
	this->stride += count * VertexBufferElement::size_of_type(gl_type);
	this->counter += count;
}

#endif