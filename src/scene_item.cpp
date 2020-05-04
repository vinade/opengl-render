#ifndef SCENE_ITEM_CPP
#define SCENE_ITEM_CPP

#include "scene_item.hpp"

void SceneItem::load_scene_from_file(const std::string &file_path)
{
	std::ifstream file_stream(file_path.c_str());

	if (file_stream.fail())
	{
		std::cerr << "[SceneItem] erro." << std::endl;
		std::cerr << "\t" << SceneItem::assimp_importer.GetErrorString() << std::endl;
		exit(1);
	}
	file_stream.close();

	this->scene = SceneItem::assimp_importer.ReadFile(file_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
	if (!this->scene)
	{
		std::cerr << "[SceneItem] erro." << std::endl;
		std::cerr << "\t" << SceneItem::assimp_importer.GetErrorString() << std::endl;
		exit(1);
	}

	this->model_path = std::string(file_path);

	size_t slash_pos = file_path.find_last_of("\\/");
	this->base_path = std::string("");
	if (std::string::npos != slash_pos)
	{
		this->base_path = file_path.substr(0, slash_pos + 1);
	}
}

void SceneItem::load_material_textures(aiMaterial *material, aiTextureType tex_type)
{
	int counter = material->GetTextureCount(tex_type);
	for (int tex_index = 0; tex_index < counter; tex_index++)
	{
		aiString tex_path;
		material->GetTexture(tex_type, tex_index, &tex_path);
		Texture *tex = new Texture(this->base_path + tex_path.data, tex_type);
	}
}

void SceneItem::load_scene_textures()
{

	if (!this->scene)
	{
		std::cerr << "[SceneItem] Tentativa de carregamento de texturas sem uma cena válida." << std::endl;
		exit(1);
	}

	if (this->scene->HasTextures())
	{
		std::cerr << "[SceneItem] a cena já tem texturas. ------- DEBUG." << std::endl;
		// a cena já contém as texturas
		return;
	}

	for (int i = 0; i < this->scene->mNumMaterials; i++)
	{
		for (int tex_type_index = 0; tex_type_index < SCENE_ITEM_TEX_TYPE_COUNTER; tex_type_index++)
		{
			SceneItem::load_material_textures(this->scene->mMaterials[i], SceneItem::texture_types[tex_type_index]);
		}
	}
}

inline void SceneItem::get_min_max(aiVector3D &value, int i)
{

	if (this->min_max_undefined || value[i] < this->min[i])
	{
		this->min[i] = value[i];
	}

	if (this->min_max_undefined || value[i] > this->max[i])
	{
		this->max[i] = value[i];
	}
}

inline void SceneItem::calculate_coords()
{

	this->center.x = (this->min.x + this->max.x) / 2.0;
	this->center.y = (this->min.y + this->max.y) / 2.0;
	this->center.z = (this->min.z + this->max.z) / 2.0;

	this->size.x = this->max.x - this->min.x;
	this->size.y = this->max.y - this->min.y;
	this->size.z = this->max.z - this->min.z;
}

void SceneItem::debug_coords()
{
	std::cerr << "DEBUG COORDS\n";

	std::cerr << "this->center.x: " << this->center.x << "\n";
	std::cerr << "this->center.y: " << this->center.y << "\n";
	std::cerr << "this->center.z: " << this->center.z << "\n";
	std::cerr << "this->size.x: " << this->size.x << "\n";
	std::cerr << "this->size.y: " << this->size.y << "\n";
	std::cerr << "this->size.z: " << this->size.z << "\n";
	std::cerr << "this->min.x: " << this->min.x << "\n";
	std::cerr << "this->min.y: " << this->min.y << "\n";
	std::cerr << "this->min.z: " << this->min.z << "\n";
	std::cerr << "this->max.x: " << this->max.x << "\n";
	std::cerr << "this->max.y: " << this->max.y << "\n";
	std::cerr << "this->max.z: " << this->max.z << "\n\n";
}

/*
	TODO:
	ESTÁ ACEITANDO APENAS UMA TEXTURA, E IGNORANDO OS MATERIAIS

*/
void SceneItem::collect_vertex_data(const struct aiScene *sc, const struct aiNode *nd)
{
	Mesh mesh_data(8);

	for (int n = 0; n < nd->mNumMeshes; n++)
	{
		const struct aiMesh *mesh = sc->mMeshes[nd->mMeshes[n]];
		const aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];
		aiString texPath;

		if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath))
		{
			Texture *tex = new Texture(this->base_path + texPath.data, aiTextureType_DIFFUSE);
			mesh_data.textures.push_back(tex);
		}

		for (int vi = 0; vi < mesh->mNumVertices; vi++)
		{
			std::vector<float> vertex;

			vertex.push_back(mesh->mVertices[vi].x);
			vertex.push_back(mesh->mVertices[vi].y);
			vertex.push_back(mesh->mVertices[vi].z);

			this->get_min_max(mesh->mVertices[vi], 0); // X
			this->get_min_max(mesh->mVertices[vi], 1); // Y
			this->get_min_max(mesh->mVertices[vi], 2); // Z
			this->min_max_undefined = false;

			if (this->min_max_undefined || mesh->mVertices[vi].z < this->min.z)
			{
				this->min.z = mesh->mVertices[vi].z;
			}

			if (this->min_max_undefined || mesh->mVertices[vi].y > this->max.z)
			{
				this->max.z = mesh->mVertices[vi].z;
			}

			if (mesh->HasTextureCoords(0))
			{
				vertex.push_back(mesh->mTextureCoords[0][vi].x);
				vertex.push_back(mesh->mTextureCoords[0][vi].y);
			}

			vertex.push_back(mesh->mNormals[vi].x);
			vertex.push_back(mesh->mNormals[vi].y);
			vertex.push_back(mesh->mNormals[vi].z);

			// if (mesh->HasTextureCoords(1)){
			// 	vertex.push_back(mesh->mTextureCoords[1][vi].x);
			// 	vertex.push_back(mesh->mTextureCoords[1][vi].y);
			// }

			if (mesh->mColors[0] != NULL)
			{
				vertex.push_back(mesh->mColors[0][vi].r);
				vertex.push_back(mesh->mColors[0][vi].g);
				vertex.push_back(mesh->mColors[0][vi].b);
				vertex.push_back(mesh->mColors[0][vi].a);
			}

			mesh_data.vertex_data.push_back(vertex);
		}

		for (int fi = 0; fi < mesh->mNumFaces; fi++)
		{
			const struct aiFace *face = &mesh->mFaces[fi];

			switch (face->mNumIndices)
			{
			// case 1: // GL_POINTS
			// case 2: // GL_LINES
			case 3: // GL_TRIANGLES
				mesh_data.index_data.push_back(face->mIndices[0]);
				mesh_data.index_data.push_back(face->mIndices[1]);
				mesh_data.index_data.push_back(face->mIndices[2]);
				break;
			case 4: // GL_QUAD
				mesh_data.index_data.push_back(face->mIndices[0]);
				mesh_data.index_data.push_back(face->mIndices[1]);
				mesh_data.index_data.push_back(face->mIndices[2]);

				mesh_data.index_data.push_back(face->mIndices[2]);
				mesh_data.index_data.push_back(face->mIndices[3]);
				mesh_data.index_data.push_back(face->mIndices[0]);
				break;
			// default: // GL_POLYGON
			default:
				std::cerr << "[SceneItem] número de lados não suportados: " << face->mNumIndices << std::endl;
				exit(1);
			}
		}
	}

	mesh_data.vertex_count = mesh_data.vertex_data.size();
	mesh_data.index_count = mesh_data.index_data.size();

	if (mesh_data.vertex_data.size() > 0)
	{
		this->meshes.push_back(mesh_data);
	}

	for (int n = 0; n < nd->mNumChildren; n++)
	{
		SceneItem::collect_vertex_data(sc, nd->mChildren[n]);
	}
}

void SceneItem::load_data_from_file(const std::string &file_path)
{
	this->load_scene_from_file(file_path);
	this->load_scene_textures();
	this->collect_vertex_data(this->scene, this->scene->mRootNode);
	this->calculate_coords(); // calcula o centro e tamanho do objeto

	// this->debug_coords();

	for (int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].prepare(this->center, this->size);
	}
}

void SceneItem::draw()
{
	this->update_model_matrix();
	for (int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].draw(this->model_matrix);
	}
}

void SceneItem::update_model_matrix()
{

	// pela normalizacao, o objeto já se encontra no p(0,0,0)

	this->model_matrix = glm::translate(glm::mat4(1.0f), this->m_position);
	this->model_matrix = glm::scale(this->model_matrix, this->m_scale);
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::vec3 SceneItem::get_position()
{
	return glm::vec3(this->m_position[0], this->m_position[2], this->m_position[2]);
}

void SceneItem::set_position(const glm::vec3 position)
{
	this->m_position[0] = position[0];
	this->m_position[1] = position[1];
	this->m_position[2] = position[2];
}

void SceneItem::set_rotation(const glm::vec3 rotation)
{
	this->m_rotation[0] = rotation[0];
	this->m_rotation[1] = rotation[1];
	this->m_rotation[2] = rotation[2];
}

void SceneItem::set_scale(const glm::vec3 scale)
{
	this->m_scale[0] = scale[0];
	this->m_scale[1] = scale[1];
	this->m_scale[2] = scale[2];
}

void SceneItem::set_scale(float scale)
{
	this->m_scale[0] = scale;
	this->m_scale[1] = scale;
	this->m_scale[2] = scale;
}

void SceneItem::inc_position(const glm::vec3 position)
{
	this->m_position[0] = this->m_position[0] + position[0];
	this->m_position[1] = this->m_position[1] + position[1];
	this->m_position[2] = this->m_position[2] + position[2];
}

void SceneItem::inc_rotation(const glm::vec3 rotation)
{
	this->m_rotation[0] = this->m_rotation[0] + rotation[0];
	this->m_rotation[1] = this->m_rotation[1] + rotation[1];
	this->m_rotation[2] = this->m_rotation[2] + rotation[2];
}

void SceneItem::inc_scale(const glm::vec3 scale)
{
	this->m_scale[0] = this->m_scale[0] + scale[0];
	this->m_scale[1] = this->m_scale[1] + scale[1];
	this->m_scale[2] = this->m_scale[2] + scale[2];
}

void SceneItem::inc_scale(float scale)
{
	this->m_scale[0] = this->m_scale[0] + scale;
	this->m_scale[1] = this->m_scale[1] + scale;
	this->m_scale[2] = this->m_scale[2] + scale;
}

aiTextureType SceneItem::texture_types[SCENE_ITEM_TEX_TYPE_COUNTER] = {
	aiTextureType_NONE,
	aiTextureType_DIFFUSE,
	aiTextureType_UNKNOWN,
	aiTextureType_DISPLACEMENT,
	aiTextureType_SPECULAR,
	aiTextureType_AMBIENT,
	aiTextureType_EMISSIVE,
	aiTextureType_NORMALS,
	aiTextureType_OPACITY,
	aiTextureType_SHININESS,
	aiTextureType_HEIGHT,
	aiTextureType_LIGHTMAP,
	aiTextureType_REFLECTION};

Assimp::Importer SceneItem::assimp_importer;

#endif
