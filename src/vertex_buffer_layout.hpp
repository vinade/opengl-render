#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

#include <vector>
#include "data_types.hpp"


typedef struct str_vertex_buffer_element
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int size_of_type(unsigned int type){
		switch(type){
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}

		return 0;
	}

} VertexBufferElement;


class VertexBufferLayout {

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
public:

	VertexBufferLayout():
		stride(0){};

	// template<typename T>
	// void push(int count){ };

	// template<>
	// void push<float>(int count);

	// template<>
	// void push<unsigned int>(int count);

	// template<>
	// void push<unsigned char>(int count);

	void push(DataType type, unsigned int count);

	inline const std::vector<VertexBufferElement>& get_elements() const;

	inline unsigned int get_stride() const;
};

#endif