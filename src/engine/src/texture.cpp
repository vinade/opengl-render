#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <GL/glew.h>
#include <iostream>
#include "stb/stb_image.h"
#include "texture.hpp"
#include "cmake_params.hpp"
#include "render_window.hpp"

Texture::Texture(const std::string &file_path, TextureType tex_type) : id(0),
																	   file_path(file_path),
																	   height(0),
																	   width(0),
																	   bpp(0)
{
	this->load_texture(tex_type, false);
}

Texture::Texture(const std::string &file_path, TextureType tex_type, bool preload) : id(0),
																					 file_path(file_path),
																					 height(0),
																					 width(0),
																					 bpp(0)
{
	this->load_texture(tex_type, preload);
}

Texture::Texture(unsigned int *id, int *width, int *height)
{
	this->_width = width;
	this->_height = height;
	this->_id = id;
}

void Texture::load_texture(TextureType tex_type, bool preload)
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

		this->file_path = Texture::texture_folder + this->file_path;
		this->local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->bpp, this->get_channels());

		if (!this->width || !this->height)
		{
			std::cerr << "[Texture] Não foi possível carregar a textura: " << original_file_path << std::endl;
			std::cerr << "\tfallback também falhou: " << this->file_path << std::endl;
			exit(1);
		}
	}

	if (RenderWindow::is_render_thread())
	{
		this->setup();
	}
	else
	{
		this->ready = false;
		RenderWindow::context->to_setup(this);
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
		// return 1;
	case TEXTURE_AMBIENT_OCLUSION:
	case TEXTURE_DISPLACEMENT:
	case TEXTURE_REFLECTION:
	case TEXTURE_ROUGHNESS:
	case TEXTURE_METALLIC: // TODO: verificar o numero de canais da imagem dos mapas que precisam de apenas 1 canal
	case TEXTURE_DIFFUSE:
	case TEXTURE_NORMAL:
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
		exit(1);
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
		std::cerr << "[Texture] Ponteiro nulo " << std::endl;
		exit(1);
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

	glTexStorage2D(GL_TEXTURE_2D, TEXTURE_MIPMAPS, GL_RGBA8, this->width, this->height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, this->local_buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (TEXTURE_ANISOTROPIC > 0.0)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, TEXTURE_ANISOTROPIC);
	}

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

TextureType Texture::to_texture_type(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType_AMBIENT:
	case aiTextureType_EMISSIVE:
		return TEXTURE_OTHER;
	case aiTextureType_OPACITY:
		return TEXTURE_METALLIC;
	case aiTextureType_SPECULAR:
	case aiTextureType_SHININESS:
		return TEXTURE_ROUGHNESS;
	case aiTextureType_DISPLACEMENT:
	case aiTextureType_HEIGHT:
		return TEXTURE_DISPLACEMENT;
	case aiTextureType_LIGHTMAP:
		return TEXTURE_AMBIENT_OCLUSION;
	case aiTextureType_REFLECTION:
		return TEXTURE_REFLECTION;
	case aiTextureType_NORMALS:
		return TEXTURE_NORMAL;
	case aiTextureType_NONE:
	case aiTextureType_UNKNOWN:
	case aiTextureType_DIFFUSE:
	default:
		return TEXTURE_DIFFUSE;
	}

	return TEXTURE_DIFFUSE;
}

std::unordered_map<unsigned int, Texture *> Texture::textures;
std::unordered_map<std::string, Texture *> Texture::sources;
Texture *Texture::fallback = nullptr;
const std::string Texture::texture_folder = std::string(CMAKE_ROOT_DIR TEXTURE_DEFAULT_FOLDER);

#endif