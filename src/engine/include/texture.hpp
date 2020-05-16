#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <unordered_map>
#include <assimp/material.h>

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "./"
#endif

#define TEXTURE_CHANNELS 4
#define TEXTURE_DEFAULT_FOLDER "./res/textures/"

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
	static Texture *fallback;
	static const std::string texture_folder;

	Texture(const std::string &file_path, aiTextureType tex_type = aiTextureType_DIFFUSE); // verifica antes se já existe em Texture::sources
	Texture(const unsigned int tid);													   // verifica antes se em textures
	~Texture();

	void bind() const;
	void bind(unsigned int slot) const;
	static void unbind();

	inline int get_width() const { return this->width; };
	inline int get_height() const { return this->height; };

	static inline int get_type_slot(aiTextureType type);
};

inline int Texture::get_type_slot(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType_NONE:
	case aiTextureType_DIFFUSE:
	default:
		return 1;
	case aiTextureType_NORMALS:
		return 2;
	case aiTextureType_SPECULAR:
		return 3;
	case aiTextureType_SHININESS:
		return 4;
	case aiTextureType_REFLECTION:
		return 5;
	case aiTextureType_OPACITY:
		return 6;
	case aiTextureType_UNKNOWN:
		return 7;
	case aiTextureType_AMBIENT:
		return 8;
	case aiTextureType_LIGHTMAP:
		return 9;
	case aiTextureType_HEIGHT:
		return 10;
	case aiTextureType_EMISSIVE:
		return 11;
	case aiTextureType_DISPLACEMENT:
		return 12;
	}

	return 0;
}

#endif