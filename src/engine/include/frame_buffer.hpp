#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#define FRAME_BUFFER_OFF 0

#include "texture.hpp"
// #include "scene.hpp"
#include "tile.hpp"
#include "shader.hpp"

class FrameBuffer
{

private:
    bool depth = false;
    unsigned int texture_id;
    int width;
    int height;

    Tile tile;
    void *scene;
    Texture *texture;

public:
    unsigned int id;
    std::string shader_name;
    Shader *shader;

    FrameBuffer();
    FrameBuffer(bool depth);
    void set();

    ~FrameBuffer();

    void bind() const;
    static void unbind();

    void draw();
};

#endif