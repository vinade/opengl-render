#ifndef MESH_HPP
#define MESH_HPP

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"
#include "material.hpp"
#include <vector>
#include <glm/glm.hpp>

class Mesh
{

private:
	void flat_vertex_data(VertexBufferLayout *vbo_layout);
	void normalize(glm::vec3 &center, glm::vec3 &size);

public:
	unsigned int vertex_size = 8;
	unsigned int vertex_count;
	unsigned int index_count;
	Shader *shader;
	Material material;

	Mesh(){};
	Mesh(int _vertex_size) : vertex_size(_vertex_size){};

	float *vertex_buffer;
	std::vector<std::vector<float>> vertex_data;
	std::vector<unsigned int> index_data;

	VertexArray *vao;
	IndexBuffer *ibo;

	void prepare(glm::vec3 &center, glm::vec3 &size);
	void draw(Shader *shader);
};

#endif
