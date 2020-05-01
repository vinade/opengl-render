#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <unordered_map>

#define TEXTURE_CHANNELS 4


class Texture {

private:
	unsigned int id;
	std::string file_path;
	int height;
	int width;
	int bpp;

	void load_from_tid(unsigned int tid);

	static std::unordered_map <unsigned int, Texture*> textures;
	static std::unordered_map <std::string, unsigned int> sources;

public:
	Texture(const std::string& file_path); // verifica antes se já existe em Texture::sources
	Texture(const unsigned int tid); // verifica antes se em textures
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int get_width() const { return this->width; };
	inline int get_height() const { return this->height; };

};

#endif