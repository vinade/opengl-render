#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#define FRAME_BUFFER_OFF 0

#include "texture.hpp"
#include "tile.hpp"
#include "shader.hpp"
#include <vector>

class FrameBuffer
{

private:
    bool ready = false;
    bool depth = false;

    unsigned int texture_id;

    int width;
    int height;
    int channels;

    Tile tile;
    void *scene;
    Texture *texture;

public:
    float *data;
    unsigned int id;
    std::string shader_name;
    Shader *shader;

    FrameBuffer();
    FrameBuffer(bool depth);
    ~FrameBuffer();

    void init(bool depth);
    void set();
    void draw();
    void update_data();
    void save(const std::string &file_path);
    void bind() const;
    void setup();

    static void unbind();
};

#endif