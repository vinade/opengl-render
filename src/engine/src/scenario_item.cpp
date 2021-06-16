#ifndef SCENARIO_ITEM_CPP
#define SCENARIO_ITEM_CPP

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "cmake_params.hpp"
#include "scenario_item.hpp"
#include "render_window.hpp"
#include "material_loader.hpp"
#include "height_map.hpp"

ScenarioItem::ScenarioItem(bool use_item_ref)
{
	if (use_item_ref)
	{
		this->item_ref = new ReferenceItem();
	}
}

void ScenarioItem::load_scene_from_file(const std::string &file_path)
{

	std::string complete_path = ScenarioItem::models_folder + file_path;
	std::ifstream file_stream(complete_path.c_str());

	if (file_stream.fail())
	{
		std::cerr << "[ScenarioItem] erro." << std::endl;
		std::cerr << "\t" << ScenarioItem::assimp_importer.GetErrorString() << std::endl;
		exit(1);
	}
	file_stream.close();

	this->scene = ScenarioItem::assimp_importer.ReadFile(complete_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (!this->scene)
	{
		std::cerr << "[ScenarioItem] erro." << std::endl;
		std::cerr << "\t" << ScenarioItem::assimp_importer.GetErrorString() << std::endl;
		exit(1);
	}

	this->model_path = std::string(complete_path);

	size_t slash_pos = complete_path.find_last_of("\\/");
	this->base_path = std::string("");
	if (std::string::npos != slash_pos)
	{
		this->base_path = complete_path.substr(0, slash_pos + 1);
	}
}

void ScenarioItem::load_material_textures(aiMaterial *material, aiTextureType tex_type, bool preload)
{
	int counter = material->GetTextureCount(tex_type);
	for (int tex_index = 0; tex_index < counter; tex_index++)
	{
		aiString tex_path;
		material->GetTexture(tex_type, tex_index, &tex_path);
		// new Texture(this->base_path + tex_path.data, tex_type, preload);
		/*

			TODO: fazer um DE-PARA dos tipos de textura do assimp para os da engine

		*/
		new Texture(this->base_path + tex_path.data, Texture::to_texture_type(tex_type), preload); // temporario
	}
}

void ScenarioItem::load_scene_textures(bool preload)
{

	if (!this->scene)
	{
		std::cerr << "[ScenarioItem] Tentativa de carregamento de texturas sem uma cena válida." << std::endl;
		exit(1);
	}

	if (this->scene->HasTextures())
	{
		std::cerr << "[ScenarioItem] a cena já tem texturas. ------- DEBUG." << std::endl;
		// a cena já contém as texturas
		return;
	}

	for (unsigned int i = 0; i < this->scene->mNumMaterials; i++)
	{
		for (int tex_type_index = 0; tex_type_index < SCENARIO_ITEM_TEX_TYPE_COUNTER; tex_type_index++)
		{
			ScenarioItem::load_material_textures(this->scene->mMaterials[i], ScenarioItem::texture_types[tex_type_index], preload);
		}
	}
}

inline void ScenarioItem::get_min_max(aiVector3D &value, int i)
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

inline void ScenarioItem::calculate_coords()
{

	this->center.x = (this->min.x + this->max.x) / 2.0;
	this->center.y = (this->min.y + this->max.y) / 2.0;
	this->center.z = (this->min.z + this->max.z) / 2.0;

	this->size.x = this->max.x - this->min.x;
	this->size.y = this->max.y - this->min.y;
	this->size.z = this->max.z - this->min.z;
}

void ScenarioItem::debug_coords()
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

void ScenarioItem::collect_vertex_data(const struct aiScene *sc, const struct aiNode *nd)
{
	Mesh mesh_data;

	for (unsigned int n = 0; n < nd->mNumMeshes; n++)
	{
		const struct aiMesh *mesh = sc->mMeshes[nd->mMeshes[n]];
		aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];
		aiString texPath;

		mesh_data.material = Material::create_from_aiMaterial(mtl, this->base_path);

		for (unsigned int vi = 0; vi < mesh->mNumVertices; vi++)
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

			if (!mesh->HasNormals())
			{
				// std::cerr << "[ScearioItem] Objeto sem Normais" << std::endl;
				// exit(1);

				for (int i = 0; i < 9; i++)
				{
					// fallback temporario
					vertex.push_back(0.0);
				}
			}
			else
			{
				vertex.push_back(mesh->mNormals[vi].x);
				vertex.push_back(mesh->mNormals[vi].y);
				vertex.push_back(mesh->mNormals[vi].z);

				vertex.push_back(mesh->mTangents[vi].x);
				vertex.push_back(mesh->mTangents[vi].y);
				vertex.push_back(mesh->mTangents[vi].z);

				vertex.push_back(mesh->mBitangents[vi].x);
				vertex.push_back(mesh->mBitangents[vi].y);
				vertex.push_back(mesh->mBitangents[vi].z);
			}

			mesh_data.vertex_data.push_back(vertex);
		}

		for (unsigned int fi = 0; fi < mesh->mNumFaces; fi++)
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
				fi = mesh->mNumFaces; // fallback temporário

				// std::cerr << "[ScenarioItem] número de lados não suportados: " << face->mNumIndices << std::endl;
				// exit(1);
			}
		}
	}

	mesh_data.vertex_count = mesh_data.vertex_data.size();
	mesh_data.index_count = mesh_data.index_data.size();

	if (mesh_data.vertex_data.size() > 0)
	{
		this->meshes.push_back(mesh_data);
	}

	for (unsigned int n = 0; n < nd->mNumChildren; n++)
	{
		ScenarioItem::collect_vertex_data(sc, nd->mChildren[n]);
	}
}

void ScenarioItem::load_data_from_file(const std::string &file_path)
{
	bool preload = !RenderWindow::is_render_thread();
	this->load_data_from_file(file_path, preload);
}

void ScenarioItem::load_data_from_file(const std::string &file_path, bool preload)
{
	this->load_scene_from_file(file_path);
	this->load_scene_textures(preload);
	this->collect_vertex_data(this->scene, this->scene->mRootNode);
	this->calculate_coords(); // calcula o centro e tamanho do objeto

	// this->debug_coords();

	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].prepare(this->center, this->size, preload);
	}
}

void ScenarioItem::draw(Shader *shader)
{

	if (shader->use_mvp)
	{
		this->update_model_matrix();
		shader->fill("u_Model", this->model_matrix);
	}

	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].draw(shader);
	}
}

void ScenarioItem::draw(Shader *shader, const glm::vec3 &pos, const glm::vec3 &rotation, const glm::vec3 &scale)
{

	if (shader->use_mvp)
	{
		this->update_model_matrix(pos, rotation, scale);
		shader->fill("u_Model", this->model_matrix);
	}

	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].draw(shader);
	}
}

void ScenarioItem::set_material(const std::string &material_name)
{
	Material *mtl = MaterialLoader::get_material(material_name);
	this->set_material(mtl);
}

void ScenarioItem::set_material(Material *mtl)
{
	for (auto &mesh : this->meshes)
	{
		mesh.material = mtl;
	}
}

void ScenarioItem::set_on_height_map(HeightMap *hp)
{
	float height = hp->get_height(this->m_position.x, this->m_position.z);
	this->m_position.y = height + this->m_scale.y / 2.0;
}

aiTextureType ScenarioItem::texture_types[SCENARIO_ITEM_TEX_TYPE_COUNTER] = {
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

Assimp::Importer ScenarioItem::assimp_importer;
const std::string ScenarioItem::models_folder = std::string(CMAKE_ROOT_DIR SCENARIO_ITEM_MODELS_FOLDER);

#endif
