#ifndef TILE_CPP
#define TILE_CPP

#include <glm/gtc/matrix_transform.hpp>
#include "render_window.hpp"
#include "tile.hpp"

void Tile::set()
{

    if (this->loaded)
    {
        return;
    }

    this->set(TILE_MESH_DEFAULT_TEXTURE);
}

void Tile::set(const std::string &texture_path)
{

    if (this->loaded)
    {
        return;
    }

    this->tile_mesh = new TileMesh(texture_path);
    this->loaded = true;
}

void Tile::set(Texture *texture_obj)
{

    if (this->loaded)
    {
        return;
    }

    this->tile_mesh = new TileMesh(texture_obj);
    this->loaded = true;
}

void Tile::draw()
{
    if (!this->loaded) // Se não foi carregado na hora do draw()
    {
        this->set(); // carrega config padrão
    }

    this->update_model_matrix();
    this->tile_mesh->draw(this->model_matrix);
}

void Tile::set_size_in_percent(float width, float height)
{
    this->tile_size.x = width;
    this->tile_size.y = height;
}

void Tile::set_position_in_percent(float x, float y)
{
    this->tile_position.x = x;
    this->tile_position.y = 1.0 - y;
}

void Tile::set_size_in_pixels(float width, float height)
{
    this->tile_size.x = width / RenderWindow::width;
    this->tile_size.y = height / RenderWindow::height;
}

void Tile::set_position_in_pixels(float x, float y)
{
    this->tile_position.x = x / RenderWindow::width;
    this->tile_position.y = 1.0 - y / RenderWindow::height;
}

void Tile::center_x()
{
    this->tile_position.x = 0.5 - (this->tile_size.x / 2.0);
}

void Tile::center_y()
{
    this->tile_position.y = 0.5 + (this->tile_size.y / 2.0);
}

void Tile::update_model_matrix()
{
    glm::vec3 real_position = glm::vec3(this->tile_position);
    glm::vec3 real_size = glm::vec3(this->tile_size);

    real_position.x = real_position.x * 2.0 - 1.0;
    real_position.y = real_position.y * 2.0 - 1.0;
    real_position.x = real_position.x + real_size.x; // o tamanho está na escala de 0..1 ao invés de -1..1
    real_position.y = real_position.y - real_size.y; // por isso não é necessário dividir por 2

    this->model_matrix = glm::translate(glm::mat4(1.0f), real_position);
    this->model_matrix = glm::scale(this->model_matrix, real_size);
}

#endif
