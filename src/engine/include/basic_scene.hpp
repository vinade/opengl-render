#ifndef BASIC_SCENE_HPP
#define BASIC_SCENE_HPP

#include "tile.hpp"
#include "frame_buffer.hpp"
#include "render_window.hpp"
#include <vector>
#include <algorithm>

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

    template <typename T>
    static bool add_once(std::vector<T> &item_vector, const T &item);

    template <typename T>
    static bool has_item(const std::vector<T> &item_vector, const T &item);
};

template <typename T>
bool BasicScene::has_item(const std::vector<T> &item_vector, const T &item)
{
    auto it = std::find(item_vector.begin(), item_vector.end(), item);
    return (it != item_vector.end());
}

template <typename T>
bool BasicScene::add_once(std::vector<T> &item_vector, const T &item)
{
    if (BasicScene::has_item(item_vector, item))
    {
        return false;
    }

    item_vector.push_back(item);
    return true;
}

#endif
