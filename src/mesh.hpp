#ifndef MESH_HPP
#define MESH_HPP

#include "vertex_buffer.cpp"
#include "index_buffer.cpp"
#include "texture.cpp"
#include "vertex_array.cpp"
#include "shaders.cpp"
#include <vector>

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

	Mesh(){};
	Mesh(int _vertex_size) : vertex_size(_vertex_size){};

	float *vertex_buffer;
	std::vector<std::vector<float>> vertex_data;
	std::vector<unsigned int> index_data;
	std::vector<Texture *> textures;

	VertexArray *vao;
	IndexBuffer *ibo;

	void prepare(glm::vec3 &center, glm::vec3 &size);
	void draw(glm::mat4 *mvp);
};

#endif
