#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

class IndexBuffer
{

private:
	unsigned int id;
	unsigned int count;

public:
	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();

	void bind();
	void unbind();
	void draw();
	void draw(unsigned int mode);
};

#endif