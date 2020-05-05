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

	this->type = tex_type;

	if (Texture::sources.find(file_path) != Texture::sources.end())
	{
		unsigned int tid = Texture::sources[file_path];
		this->load_from_tid(tid);
		return;
	}

	stbi_set_flip_vertically_on_load(1);

	unsigned char *local_buffer = nullptr;
	local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->bpp, this->get_channels());

	if (!this->width || !this->height)
	{
		std::cerr << "[Texture] Não foi possível carregar a textura: " << file_path << std::endl;
		exit(1);
	}

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (local_buffer)
	{
		stbi_image_free(local_buffer);
	}

	Texture::textures[this->id] = this;
	Texture::sources[file_path] = this->id;
	std::cerr << "[Texture] carregado: " << file_path.c_str() << std::endl;
	std::cerr << "\t" << this->width << "x" << this->height << std::endl;
}

Texture::Texture(unsigned int tid)
{
	this->load_from_tid(tid);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
}

// see: http://assimp.sourceforge.net/lib_html/material_8h.html#a7dd415ff703a2cc53d1c22ddbbd7dde0
inline int Texture::get_slot() const
{
	switch (this->type)
	{
	case aiTextureType_SPECULAR:
		return 1;
	case aiTextureType_AMBIENT:
		return 2;
	case aiTextureType_EMISSIVE:
		return 3;
	case aiTextureType_HEIGHT:
		return 4;
	case aiTextureType_NORMALS:
		return 5;
	case aiTextureType_SHININESS:
		return 6;
	case aiTextureType_OPACITY:
		return 7;
	case aiTextureType_DISPLACEMENT:
		return 8;
	case aiTextureType_LIGHTMAP:
		return 9;
	case aiTextureType_REFLECTION:
		return 10;
	case aiTextureType_UNKNOWN:
		return 11;
	case aiTextureType_NONE:
	case aiTextureType_DIFFUSE:
	default:
		return 0;
	}

	return 0;
}

inline int Texture::get_channels() const
{
	switch (this->type)
	{
	case aiTextureType_DISPLACEMENT:
	case aiTextureType_SPECULAR:
	case aiTextureType_AMBIENT:
	case aiTextureType_EMISSIVE:
	case aiTextureType_NORMALS:
		return 3;
	case aiTextureType_OPACITY:
	case aiTextureType_SHININESS:
	case aiTextureType_HEIGHT:
	case aiTextureType_LIGHTMAP:
	case aiTextureType_REFLECTION:
		return 1;
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
	this->width = tex->get_width();
	this->height = tex->get_height();
	this->id = tid;
}

void Texture::bind() const
{
	unsigned int slot = this->get_slot();
	this->bind(slot);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::unordered_map<unsigned int, Texture *> Texture::textures;
std::unordered_map<std::string, unsigned int> Texture::sources;

#endif