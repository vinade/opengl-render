#ifndef INDEX_BUFFER_CPP
#define INDEX_BUFFER_CPP

#include <GL/glew.h>
#include "index_buffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
{
	this->count = count;

	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &this->id);
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::draw()
{
	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, nullptr);
}

#endif