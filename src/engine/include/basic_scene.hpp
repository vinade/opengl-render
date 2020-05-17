#ifndef BASIC_SCENE_HPP
#define BASIC_SCENE_HPP

#include "tile.hpp"
#include "frame_buffer.hpp"
#include "render_window.hpp"
#include <vector>

class BasicScene
{

private:
    std::vector<Tile *> tiles;
    void draw_on_buffer(FrameBuffer *fbo);

public:
    void add(Tile *tile);
    void add(Tile &tile);

    void draw();
    void draw(FrameBuffer *fbo);
    void draw_tiles();
    void update_color_buffer(RenderWindow *render);
    void update_depth_buffer(RenderWindow *render);
};

#endif
