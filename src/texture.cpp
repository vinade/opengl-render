#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include "./3rd/stb/stb_image.cpp"
#include "texture.hpp"


Texture::Texture(const std::string& file_path) :
	id(0),
	file_path(file_path),
	local_buffer(nullptr),
	height(0),
	width(0),
	bpp(0)
{
	stbi_set_flip_vertically_on_load(1);

	this->local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->bpp, TEXTURE_CHANNELS);

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (this->local_buffer){
		stbi_image_free(this->local_buffer);
	}
}

Texture::~Texture(){
	glDeleteTextures(1, &this->id);
}

void Texture::bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

#endif