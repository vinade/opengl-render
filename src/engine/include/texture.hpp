#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <unordered_map>
#include <vector>
#include <assimp/material.h>

#define TEXTURE_CHANNELS 4
#define TEXTURE_DEFAULT_FOLDER "./res/textures/"

class Texture
{

private:
	std::string file_path;
	int *_height;
	int *_width;
	unsigned int *_id;
	int bpp;
	unsigned char *local_buffer = nullptr;

	static std::unordered_map<unsigned int, Texture *> textures;
	static std::unordered_map<std::string, Texture *> sources;

	void load_from_tid(unsigned int tid);
	void load_from_texture(Texture *tex);
	inline int get_slot() const;
	inline int get_channels() const;

public:
	int height;
	int width;
	bool ready = false;
	unsigned int id;
	aiTextureType type = aiTextureType_DIFFUSE;
	static Texture *fallback;
	static const std::string texture_folder;

	Texture(unsigned int *id, int *width, int *height);
	Texture(const std::string &file_path, aiTextureType tex_type, bool preload);
	Texture(const std::string &file_path, aiTextureType tex_type = aiTextureType_DIFFUSE); // verifica antes se já existe em Texture::sources
	~Texture();

	void load_texture(aiTextureType tex_type, bool preload);
	void setup();
	void bind() const;
	void bind(unsigned int slot) const;
	static void unbind();

	inline int get_width() const { return *this->_width; };
	inline int get_height() const { return *this->_height; };
	inline unsigned int get_id() const { return *this->_id; };

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