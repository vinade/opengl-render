#ifndef VERTEX_ARRAY_CPP
#define VERTEX_ARRAY_CPP

#include "vertex_array.hpp"


VertexArray::VertexArray(){
	glGenVertexArrays(1, &this->id);
	std::cout << "GENERATE\n";
}

VertexArray::~VertexArray(){
	glDeleteVertexArrays(1, &this->id);
}

void VertexArray::add_buffer(VertexBuffer* vbo, VertexBufferLayout* layout){

	this->bind();
	vbo->bind();

	const auto& elements = layout->get_elements();
	unsigned int stride = layout->get_stride();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++){
		const auto& element = elements[i];

	    glEnableVertexAttribArray(i); // layout position
	    glVertexAttribPointer(
	    	i,
	    	element.count,
	    	element.type,
	    	element.normalized,
	    	stride,
	    	(const void*)offset
	    );

	    offset += element.count * VertexBufferElement::size_of_type(element.type);
	}
}

void VertexArray::bind(){
	glBindVertexArray(this->id);
}

void VertexArray::unbind(){
	glBindVertexArray(0);
}


#endif