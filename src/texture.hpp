#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define TEXTURE_CHANNELS 4


class Texture {

private:
	unsigned int id;
	std::string file_path;
	unsigned char* local_buffer;
	int height;
	int width;
	int bpp;


public:
	Texture(const std::string& file_path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int get_width() const { return this->width; };
	inline int get_height() const { return this->height; };
};

#endif