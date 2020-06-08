#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#define FRAME_BUFFER_OFF 0
#define FRAME_BUFFER_FLAG_DEPTH 1
#define FRAME_BUFFER_FLAG_AUX_FBO 2
#define FRAME_BUFFER_DEPTH 1
#define FRAME_BUFFER_DEPTH_WITH_CORRECTION 3

#include "texture.hpp"
#include "tile.hpp"
#include "shader.hpp"
#include <vector>

class PostProcess;

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
    FrameBuffer *color_fbo = nullptr;

public:
    bool depth = false;
    float *data;
    unsigned int id;
    std::string shader_name;
    Shader *shader;

    FrameBuffer();
    FrameBuffer(unsigned int fbo_type);
    ~FrameBuffer();

    void init(unsigned int fbo_type);
    void set();
    void draw();
    void draw(FrameBuffer *fbo);
    void draw(PostProcess *pp_shader);
    void draw(FrameBuffer *fbo, PostProcess *pp_shader);
    void update_data();
    void update_data(float *data);
    void update_data(float *data, bool force_one_channel);
    void save(const std::string &file_path);
    void bind() const;
    void setup();

    static void unbind();
};

#endif