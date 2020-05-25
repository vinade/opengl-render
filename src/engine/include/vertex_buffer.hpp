#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

class VertexBuffer
{

private:
	unsigned int id;

public:
	VertexBuffer(const void *data, unsigned int size);
	VertexBuffer(float *data, unsigned int count);

	~VertexBuffer();

	void init(const void *data, unsigned int size);
	void update(float *data, unsigned int count);
	void bind() const;
	void unbind() const;
};

#endif