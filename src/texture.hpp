#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <unordered_map>
#include <assimp/material.h>
#include "./3rd/stb/stb_image.cpp"

#define TEXTURE_CHANNELS 4

class Texture
{

private:
	unsigned int id;
	std::string file_path;
	int height;
	int width;
	int bpp;

	static std::unordered_map<unsigned int, Texture *> textures;
	static std::unordered_map<std::string, unsigned int> sources;

	void load_from_tid(unsigned int tid);
	inline int get_slot() const;
	inline int get_channels() const;

public:
	aiTextureType type = aiTextureType_DIFFUSE;

	Texture(const std::string &file_path, aiTextureType tex_type = aiTextureType_DIFFUSE); // verifica antes se já existe em Texture::sources
	Texture(const unsigned int tid);													   // verifica antes se em textures
	~Texture();

	void bind() const;
	void bind(unsigned int slot) const;
	void unbind() const;

	inline int get_width() const { return this->width; };
	inline int get_height() const { return this->height; };
};

#endif