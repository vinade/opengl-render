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
	bool ready = false;
	unsigned int vertex_count;
	unsigned int index_count;
	Shader *shader;
	Material *material;

	Mesh();

	float *vertex_buffer;
	std::vector<std::vector<float>> vertex_data;
	std::vector<unsigned int> index_data;

	VertexArray *vao;
	IndexBuffer *ibo;

	void prepare(glm::vec3 &center, glm::vec3 &size);
	void prepare(glm::vec3 &center, glm::vec3 &size, bool preload);
	void draw(Shader *shader);
	void setup();
};

#endif
