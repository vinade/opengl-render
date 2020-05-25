#ifndef HEIGHT_MAP_MESH_CPP
#define HEIGHT_MAP_MESH_CPP

#include "stb/stb_image.h"
#include "height_map_mesh.hpp"
#include "vertex_array.hpp"
#include "index_buffer.hpp"
#include "render_window.hpp"
#include <math.h>

HeightMapMesh::HeightMapMesh()
{
    if (!Material::materials.size())
    {
        std::cerr << "[HeightMap] Criando material de fallback." << std::endl;
        Material::materials.push_back(new Material());
    }

    this->material = Material::materials[0];
}

void HeightMapMesh::from_float_array(float *data, int width, int height)
{
    float half_width = width / 2.0;
    float half_height = height / 2.0;

    this->ready = false;
    this->width = width;
    this->height = width;
    this->vertex_count = width * height;
    this->index_count = (width - 1) * (height - 1) * 6;

    if (this->vertex_buffer == nullptr)
    {
        this->vertex_buffer = (float *)malloc(sizeof(float) * this->vertex_count * this->vertex_elements_count);
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int i = y * width + x;
            int vi = i * this->vertex_elements_count;
            this->vertex_buffer[vi + 0] = float(x) - half_width;
            this->vertex_buffer[vi + 1] = float(data[i]) * 2.0 - 1.0;
            this->vertex_buffer[vi + 2] = float(y) - half_height;

            float aux;
            float tex_coord_x = this->texture_size * float(x) / float(width);
            float tex_coord_y = this->texture_size * float(y) / float(height);
            tex_coord_x = modf(tex_coord_x, &aux);
            tex_coord_y = modf(tex_coord_y, &aux);

            this->vertex_buffer[vi + 3] = tex_coord_x;
            this->vertex_buffer[vi + 4] = tex_coord_y;

            this->vertex_buffer[vi + 5] = 0.0; // mantendo simples
            this->vertex_buffer[vi + 6] = 1.0; // normal para cima.
            this->vertex_buffer[vi + 7] = 0.0;

            // TB
            this->vertex_buffer[vi + 8] = 0.0;
            this->vertex_buffer[vi + 9] = 0.0;
            this->vertex_buffer[vi + 10] = 0.0;
            this->vertex_buffer[vi + 11] = 0.0;
            this->vertex_buffer[vi + 12] = 0.0;
            this->vertex_buffer[vi + 13] = 0.0;
        }
    }

    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int v0 = y * width + x;
            int v1 = v0 + width;
            int v2 = v0 + 1;
            int v3 = v1 + 1;

            this->index_data.push_back(v0);
            this->index_data.push_back(v1);
            this->index_data.push_back(v2);

            this->index_data.push_back(v2);
            this->index_data.push_back(v1);
            this->index_data.push_back(v3);
        }
    }

    this->setup();
}

void HeightMapMesh::setup()
{

    if (this->ready)
    {
        return;
    }

    if (!RenderWindow::is_render_thread())
    {
        RenderWindow::context->to_setup(this);
        return;
    }
    this->ready = true;

    if (this->vbo != nullptr)
    {
        this->vbo->update(this->vertex_buffer, this->vertex_count * vbo_layout->get_counter());
        return;
    }

    this->vao = new VertexArray();

    if (this->vbo_layout == nullptr)
    {
        this->vbo_layout = new VertexBufferLayout();
        vbo_layout->push(DATA_TYPE_FLOAT, 3);
        vbo_layout->push(DATA_TYPE_FLOAT, 2);
        vbo_layout->push(DATA_TYPE_FLOAT, 3);
        vbo_layout->push(DATA_TYPE_FLOAT, 3);
        vbo_layout->push(DATA_TYPE_FLOAT, 3);
    }

    this->vbo = new VertexBuffer(this->vertex_buffer, this->vertex_count * vbo_layout->get_counter());
    this->vao->add_buffer(this->vbo, vbo_layout);
    this->ibo = new IndexBuffer(&this->index_data[0], this->index_count);
}

void HeightMapMesh::load(const std::string &file_path)
{
    stbi_set_flip_vertically_on_load(0);

    int bpp;
    float *local_buffer;
    std::string hp_path = file_path;
    this->width = 0;
    this->height = 0;

    local_buffer = stbi_loadf(hp_path.c_str(), &this->width, &this->height, &bpp, 1);

    if (!this->width || !this->height)
    {
        std::string original_file_path = hp_path;

        hp_path = HeightMapMesh::height_map_folder + hp_path;
        local_buffer = stbi_loadf(hp_path.c_str(), &this->width, &this->height, &bpp, 1);

        if (!this->width || !this->height)
        {
            std::cerr << "[HeightMapMesh] Não foi possível carregar o height map: " << original_file_path << std::endl;
            std::cerr << "\tfallback também falhou: " << hp_path << std::endl;
            exit(1);
        }
    }

    this->from_float_array(local_buffer, this->width, this->height);
    free(local_buffer);
}

const std::string HeightMapMesh::height_map_folder = std::string(CMAKE_ROOT_DIR HEIGHT_MAP_MESH_DEFAULT_FOLDER);

#endif