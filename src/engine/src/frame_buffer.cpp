#ifndef FRAME_BUFFER_CPP
#define FRAME_BUFFER_CPP

#include <GL/glew.h>
#include "render_window.hpp"
#include "frame_buffer.hpp"
#include "scene.hpp"

FrameBuffer::FrameBuffer()
{
    this->depth = false;
    this->texture = new Texture(&this->texture_id, &this->width, &this->height);
    this->shader_name = "fbo_color";
}

FrameBuffer::FrameBuffer(bool depth)
{
    this->depth = depth;
    this->texture = new Texture(&this->texture_id, &this->width, &this->height);
    this->shader_name = "fbo_depth";
}

void FrameBuffer::set()
{
    /*
        TODO: setup do shader
    */
    this->shader = new Shader(this->shader_name);

    this->tile.set(this->texture);
    this->tile.tile_mesh->shader = this->shader;
    this->shader->setup("u_Model", DATA_TYPE_MAT4);
    this->shader->setup("u_Texture", DATA_TYPE_INT);

    this->scene = (void *)new Scene();
    ((Scene *)this->scene)->init();
    ((Scene *)this->scene)->add(this->tile);

    this->width = RenderWindow::width; // Tamanhos não atualizáveis
    this->height = RenderWindow::height;

    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);

    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    if (this->depth)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, RenderWindow::width, RenderWindow::height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->texture_id, 0);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, RenderWindow::width, RenderWindow::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture_id, 0);

        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RenderWindow::width, RenderWindow::height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "[FrameBufferObject] Erro ao criar o framebuffer" << std::endl;
        exit(1);
    }

    FrameBuffer::unbind();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteBuffers(1, &this->id);
    glDeleteTextures(1, &this->texture_id);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void FrameBuffer::draw()
{
    FrameBuffer::unbind();

    this->tile.center_x();
    this->tile.center_y();
    ((Scene *)this->scene)->draw_tiles();
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif