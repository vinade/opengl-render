#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>

// typedef struct str_vertex_data
// {
// 	glm::vec3 position;
// 	glm::vec3 normal;
// 	glm::vec2 uv;
// } VertexData;


class VertexBuffer {

private:
	unsigned int id;

public:
	// std::vector<glm::vec3> positions;
	// std::vector<glm::vec3> normals;
	// std::vector<glm::vec2> uv;

	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(float* data, unsigned int count);

	// VertexBuffer(VertexData* data, unsigned int size);
	// VertexBuffer(float* positions, float* normals, float* uv, unsigned int size);

	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

#endif