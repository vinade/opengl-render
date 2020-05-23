#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#define FRAME_BUFFER_OFF 0

#include "texture.hpp"
#include "tile.hpp"
#include "shader.hpp"
#include "post_process.hpp"
#include <vector>

class FrameBuffer
{

private:
    bool ready = false;

    unsigned int texture_id;

    int width;
    int height;
    int channels;

    Tile tile;
    void *scene;
    Texture *texture;

public:
    bool depth = false;
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
    void draw(FrameBuffer *fbo);
    void draw(FrameBuffer *fbo, PostProcess *pp_shader);
    void update_data();
    void save(const std::string &file_path);
    void bind() const;
    void setup();

    static void unbind();
};

#endif