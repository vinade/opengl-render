#ifndef FRAME_BUFFER_CPP
#define FRAME_BUFFER_CPP

#include <GL/glew.h>
#include "render_window.hpp"
#include "frame_buffer.hpp"
#include "basic_scene.hpp"

FrameBuffer::FrameBuffer()
{
    this->init(false);
}

FrameBuffer::FrameBuffer(bool depth)
{
    this->init(depth);
}

void FrameBuffer::init(bool depth)
{
    this->depth = depth;
    this->texture = new Texture(&this->texture_id, &this->width, &this->height);
    this->shader_name = depth ? "fbo_depth" : "std";
    this->channels = depth ? 1 : 3;
}

void FrameBuffer::set()
{
    this->shader = new Shader(this->shader_name, TILE_MESH_SIMPLE_SHADER);

    this->tile.set(this->texture);

    this->scene = (void *)new BasicScene();
    ((BasicScene *)this->scene)->add(this->tile);

    this->width = RenderWindow::context->width; // Tamanhos não atualizáveis
    this->height = RenderWindow::context->height;
    this->data = (float *)malloc(this->width * this->height * this->channels * sizeof(float));

    if (!RenderWindow::is_render_thread())
    {
        RenderWindow::context->to_setup(this);
        return;
    }

    this->setup();
}

FrameBuffer::~FrameBuffer()
{
    if (this->ready)
    {
        glDeleteBuffers(1, &this->id);
        glDeleteTextures(1, &this->texture_id);
        free(this->data);
    }
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FrameBuffer::draw()
{
    this->draw(nullptr);
}

void FrameBuffer::draw(FrameBuffer *fbo)
{
    this->draw(fbo, nullptr);
}

void FrameBuffer::draw(FrameBuffer *fbo, PostProcess *pp_shader)
{

    if (fbo == nullptr)
    {
        FrameBuffer::unbind();
    }
    else
    {
        fbo->bind();
    }

    this->tile.center_x();
    this->tile.center_y();

    if ((pp_shader != nullptr) && (pp_shader->initialized))
    {
        pp_shader->model_matrix = this->tile.model_matrix;

        pp_shader->fill();
        pp_shader->exec();

        ((BasicScene *)this->scene)->tiles[0]->tile_mesh->draw();
    }
    else
    {
        ((BasicScene *)this->scene)->draw_tiles();
    }
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FrameBuffer::update_data()
{
    // torna o fbo ativo
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    // usa o FBO ativo
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    if (!this->depth)
    {
        glReadPixels(0, 0, this->width, this->height, GL_RGB, GL_FLOAT, this->data);
    }
    else
    {
        glReadPixels(0, 0, this->width, this->height, GL_DEPTH_COMPONENT, GL_FLOAT, this->data);
    }
}

void FrameBuffer::save(const std::string &file_path)
{
    FILE *output_image;
    float *pixels = this->data;

    this->update_data();

    output_image = fopen(file_path.c_str(), "wt");
    fprintf(output_image, "P3\n");
    fprintf(output_image, "%d %d\n", this->width, this->height);
    fprintf(output_image, "255\n");

    for (int j = this->height - 1; j >= 0; j--)
    {
        for (int i = 0; i < this->width; i++)
        {
            int k;
            unsigned int r, g, b;

            k = (j * this->width) + i;
            if (channels == 3)
            {
                k = k * 3;
                r = static_cast<unsigned int>(pixels[k] * 255.0);
                g = static_cast<unsigned int>(pixels[k + 1] * 255.0);
                b = static_cast<unsigned int>(pixels[k + 2] * 255.0);
            }
            else
            {
                r = static_cast<unsigned int>(pixels[k] * 255.0);
                g = r;
                b = r;
            }
            fprintf(output_image, "%u %u %u ", r, g, b);
        }
        fprintf(output_image, "\n");
    }
}

void FrameBuffer::setup()
{
    this->tile.tile_mesh->shader = this->shader;
    this->shader->setup("u_Model", DATA_TYPE_MAT4);
    this->shader->setup("u_Texture", DATA_TYPE_INT);

    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    if (this->depth)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, RenderWindow::context->width, RenderWindow::context->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->texture_id, 0);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, RenderWindow::context->width, RenderWindow::context->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture_id, 0);

        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RenderWindow::context->width, RenderWindow::context->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "[FrameBufferObject] Erro ao criar o framebuffer" << std::endl;
        exit(1);
    }

    FrameBuffer::unbind();

    this->ready = true;
}

#endif