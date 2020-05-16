#ifndef TILE_CPP
#define TILE_CPP

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

#endif
