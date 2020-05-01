#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include "./3rd/stb/stb_image.cpp"
#include "texture.hpp"


Texture::Texture(const std::string& file_path) :
	id(0),
	file_path(file_path),
	height(0),
	width(0),
	bpp(0)
{

	if (Texture::sources.find(file_path) != Texture::sources.end()){
		unsigned int tid = Texture::sources[file_path];
		this->load_from_tid(tid);
		return;
	}

	stbi_set_flip_vertically_on_load(1);

	unsigned char* local_buffer = nullptr;
	local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->bpp, TEXTURE_CHANNELS);

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (local_buffer){
		stbi_image_free(local_buffer);
	}

	Texture::textures[this->id] = this;
	Texture::sources[file_path] = this->id;
}


Texture::Texture(unsigned int tid){
	this->load_from_tid(tid);
}


Texture::~Texture(){
	glDeleteTextures(1, &this->id);
}


void Texture::load_from_tid(unsigned int tid){
	Texture* tex;

	if (Texture::textures.find(tid) == Texture::textures.end()){
		std::cerr << "[Texture] Id não encontrado: " << tid << std::endl;
		return;
	}

	tex = Texture::textures[tid];
	this->width = tex->get_width();
	this->height = tex->get_height();
	this->id = tid;
}


void Texture::bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->id);
}


void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}


std::unordered_map <unsigned int, Texture*> Texture::textures;
std::unordered_map <std::string, unsigned int> Texture::sources;

#endif