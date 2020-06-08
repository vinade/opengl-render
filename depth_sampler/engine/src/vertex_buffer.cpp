#ifndef VERTEX_BUFFER_CPP
#define VERTEX_BUFFER_CPP

#include <GL/glew.h>
#include <iostream>
#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
	this->init(data, size);
}

VertexBuffer::VertexBuffer(float *data, unsigned int count)
{
	this->init((void *)data, count * sizeof(float));
}

void VertexBuffer::init(const void *data, unsigned int size)
{
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::update(float *data, unsigned int count)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &this->id);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif