#ifndef BASIC_SCENE_CPP
#define BASIC_SCENE_CPP

#include "basic_scene.hpp"
#include "app_utils.hpp"

void BasicScene::add(Tile &tile)
{
    BasicScene::add((Tile *)&tile);
}

void BasicScene::add(Tile *tile)
{
    AppUtils::add_once(this->tiles, tile);
}

void BasicScene::update_color_buffer(RenderWindow *render)
{
    this->draw_on_buffer(render->fbo_color);
}

void BasicScene::update_depth_buffer(RenderWindow *render)
{
    this->draw_on_buffer(render->fbo_depth);
}

void BasicScene::draw_on_buffer(FrameBuffer *fbo)
{

    if (fbo == nullptr)
    {
        std::cerr << "[BasicScene/Scene] draw_on_buffer() em um ponteiro nulo." << std::endl;
        exit(1);
    }

    if (fbo->data == nullptr)
    {
        fbo->set();
    }

    this->draw(fbo);
}

void BasicScene::draw()
{
    this->draw(nullptr);
}

void BasicScene::draw(FrameBuffer *fbo)
{

    /*
        Bind FrameBuffer
    */
    if (fbo == nullptr)
    {
        FrameBuffer::unbind();
    }
    else
    {
        fbo->bind();
    }

    this->draw_tiles();
}

void BasicScene::draw_tiles()
{
    for (auto tile : this->tiles)
    {
        tile->draw();
    }
}
#endif
