#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <unordered_map>
#include <vector>
#include <assimp/material.h>

#define TEXTURE_CHANNELS 4
#define TEXTURE_DEFAULT_FOLDER "./res/textures/"

enum TextureType
{
	TEXTURE_DIFFUSE,
	TEXTURE_NORMAL,
	TEXTURE_METALLIC,
	TEXTURE_ROUGHNESS,
	TEXTURE_AMBIENT_OCLUSION,
	TEXTURE_REFLECTION,
	TEXTURE_DISPLACEMENT,
	TEXTURE_OTHER
};

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
	TextureType type = TEXTURE_DIFFUSE;
	static Texture *fallback;
	static const std::string texture_folder;

	Texture(unsigned int *id, int *width, int *height);
	Texture(const std::string &file_path, TextureType tex_type, bool preload);
	Texture(const std::string &file_path, TextureType tex_type = TEXTURE_DIFFUSE); // verifica antes se já existe em Texture::sources
	~Texture();

	void load_texture(TextureType tex_type, bool preload);
	void setup();
	void bind() const;
	void bind(unsigned int slot) const;
	static void unbind();

	inline int get_width() const { return *this->_width; };
	inline int get_height() const { return *this->_height; };
	inline unsigned int get_id() const { return *this->_id; };
	static TextureType to_texture_type(aiTextureType type);

	static inline int get_type_slot(TextureType type);
};

inline int Texture::get_type_slot(TextureType type)
{
	switch (type)
	{
	case TEXTURE_DIFFUSE:
	default:
		return 1;
	case TEXTURE_NORMAL:
		return 2;
	case TEXTURE_METALLIC:
		return 3;
	case TEXTURE_ROUGHNESS:
		return 4;
	case TEXTURE_AMBIENT_OCLUSION:
		return 5;
	case TEXTURE_REFLECTION:
		return 6;
	case TEXTURE_DISPLACEMENT:
		return 7;
	case TEXTURE_OTHER:
		return 8;
	}

	return 0;
}

#endif