#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <GL/glew.h>
#include <iostream>
#include "stb/stb_image.h"
#include "texture.hpp"

Texture::Texture(const std::string &file_path, aiTextureType tex_type) : id(0),
																		 file_path(file_path),
																		 height(0),
																		 width(0),
																		 bpp(0)
{
	this->load_texture(tex_type, false);
}

Texture::Texture(const std::string &file_path, aiTextureType tex_type, bool preload) : id(0),
																					   file_path(file_path),
																					   height(0),
																					   width(0),
																					   bpp(0)
{
	this->load_texture(tex_type, preload);
}

void Texture::load_texture(aiTextureType tex_type, bool preload)
{

	this->type = tex_type;

	if (Texture::sources.find(this->file_path) != Texture::sources.end())
	{
		Texture *tex_obj = Texture::sources[this->file_path];
		this->load_from_texture(tex_obj);
		return;
	}

	stbi_set_flip_vertically_on_load(1);

	this->local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->bpp, this->get_channels());
	if (!this->width || !this->height)
	{
		std::string original_file_path = this->file_path;

		this->file_path = this->texture_folder + this->file_path;
		this->local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->bpp, this->get_channels());

		if (!this->width || !this->height)
		{
			std::cerr << "[Texture] Não foi possível carregar a textura: " << original_file_path << std::endl;
			std::cerr << "\tfallback também falhou: " << this->file_path << std::endl;
			exit(1);
		}
	}

	if (!preload)
	{
		this->setup();
	}
	else
	{
		this->ready = false;
		Texture::to_setup.push_back(this);
	}

	Texture::sources[file_path] = this;
	std::cerr << "[Texture] carregado: " << file_path.c_str() << std::endl;
	std::cerr << "\t" << this->width << "x" << this->height << std::endl;

	if (Texture::fallback == nullptr)
	{
		Texture::fallback = this;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
}

// see: http://assimp.sourceforge.net/lib_html/material_8h.html#a7dd415ff703a2cc53d1c22ddbbd7dde0
inline int Texture::get_slot() const
{
	return Texture::get_type_slot(this->type);
}

inline int Texture::get_channels() const
{
	switch (this->type)
	{
	case aiTextureType_DISPLACEMENT:
	case aiTextureType_SPECULAR:
	case aiTextureType_AMBIENT:
	case aiTextureType_EMISSIVE:
		return 3;
	case aiTextureType_OPACITY:
	case aiTextureType_SHININESS:
	case aiTextureType_HEIGHT:
	case aiTextureType_LIGHTMAP:
	case aiTextureType_REFLECTION:
		return 1;
	case aiTextureType_NORMALS:
	case aiTextureType_NONE:
	case aiTextureType_DIFFUSE:
	case aiTextureType_UNKNOWN:
	default:
		return 4;
	}

	return 4;
}

void Texture::load_from_tid(unsigned int tid)
{
	Texture *tex;

	if (Texture::textures.find(tid) == Texture::textures.end())
	{
		std::cerr << "[Texture] Id não encontrado: " << tid << std::endl;
		return;
	}

	tex = Texture::textures[tid];
	this->_width = &tex->width;
	this->_height = &tex->height;
	this->_id = &tex->id;
}

void Texture::load_from_texture(Texture *tex)
{
	if (tex == nullptr)
	{
		std::cerr << "[Texture] Pointeiro nulo " << std::endl;
		return;
	}

	this->_width = &tex->width;
	this->_height = &tex->height;
	this->_id = &tex->id;
}

void Texture::setup()
{

	if (this->ready)
	{
		return;
	}
	this->ready = true;

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (this->local_buffer)
	{
		stbi_image_free(this->local_buffer);
	}

	Texture::textures[this->id] = this;
	this->_id = &this->id;
	this->_width = &this->width;
	this->_height = &this->height;
}

void Texture::bind() const
{
	unsigned int slot = this->get_slot();
	this->bind(slot);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->get_id());
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setup_group()
{
	for (auto item : Texture::to_setup)
	{
		item->setup();
	}
	Texture::to_setup.erase(Texture::to_setup.begin(), Texture::to_setup.end());
}

std::unordered_map<unsigned int, Texture *> Texture::textures;
std::unordered_map<std::string, Texture *> Texture::sources;
std::vector<Texture *> Texture::to_setup;
Texture *Texture::fallback = nullptr;
const std::string Texture::texture_folder = std::string(CMAKE_ROOT_DIR TEXTURE_DEFAULT_FOLDER);

#endif