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
            float tex_coord_x = float(x) / float(width);
            float tex_coord_y = float(y) / float(height);
            int i = y * width + x;
            int vi = i * this->vertex_elements_count;

            this->vertex_buffer[vi + 0] = float(x) - half_width;
            this->vertex_buffer[vi + 1] = float(data[i]) * 2.0 - 1.0;
            this->vertex_buffer[vi + 2] = float(y) - half_height;

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

float unit_rand()
{
    float res;

    res = rand() % 1000;
    res = res / 500 - 1;

    return res;
}

inline int HeightMapMesh::address_of_coord(glm::ivec2 p)
{
    return (p.y * this->width) + p.x;
}

void HeightMapMesh::diamond_square_recursion(glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3, float range, float *data, char *flag)
{

    float new_range;
    std::vector<int> p_list;

    p_list.push_back(this->address_of_coord(p0));
    p_list.push_back(this->address_of_coord(p1));
    p_list.push_back(this->address_of_coord(p2));
    p_list.push_back(this->address_of_coord(p3));

    for (int coord : p_list)
    {
        if (flag[coord] == 0)
        {
            data[coord] = data[coord] + (unit_rand() * range);
            flag[coord] = 1;
        }

        if (data[coord] > this->max_height)
            this->max_height = data[coord];

        if (data[coord] < this->min_height)
            this->min_height = data[coord];
    }

    if (((p1.x - p0.x) > 1) || ((p2.y - p0.y) > 1))
    {
        glm::ivec2 p01;
        glm::ivec2 p02;
        glm::ivec2 p03;
        glm::ivec2 p13;
        glm::ivec2 p23;

        p01.x = (p1.x + p0.x) >> 1;
        p01.y = p0.y;

        p02.x = p0.x;
        p02.y = (p2.y + p0.y) >> 1;

        p03.x = (p3.x + p0.x) >> 1;
        p03.y = (p3.y + p0.y) >> 1;

        p13.x = p1.x;
        p13.y = (p3.y + p1.y) >> 1;

        p23.x = (p3.x + p2.x) >> 1;
        p23.y = p2.y;

        int p01_coord = this->address_of_coord(p01);
        int p02_coord = this->address_of_coord(p02);
        int p03_coord = this->address_of_coord(p03);
        int p13_coord = this->address_of_coord(p13);
        int p23_coord = this->address_of_coord(p23);

        //p01
        if (flag[p01_coord] == 0)
        {
            data[p01_coord] = (data[p_list[0]] + data[p_list[1]]) / 2;
        }

        //p02
        if (flag[p02_coord] == 0)
        {
            data[p02_coord] = (data[p_list[0]] + data[p_list[2]]) / 2;
        }

        //p03
        if (flag[p03_coord] == 0)
        {
            data[p03_coord] = (data[p_list[0]] + data[p_list[3]]) / 2;
        }

        //p13
        if (flag[p13_coord] == 0)
        {
            data[p13_coord] = (data[p_list[3]] + data[p_list[1]]) / 2;
        }

        //p23
        if (flag[p23_coord] == 0)
        {
            data[p23_coord] = (data[p_list[2]] + data[p_list[3]]) / 2;
        }

        new_range = range / this->suavity;
        this->diamond_square_recursion(p0, p01, p02, p03, new_range, data, flag);
        this->diamond_square_recursion(p01, p1, p03, p13, new_range, data, flag);
        this->diamond_square_recursion(p02, p03, p2, p23, new_range, data, flag);
        this->diamond_square_recursion(p03, p13, p23, p3, new_range, data, flag);
    }
    // passo
}

void HeightMapMesh::diamond_square()
{
    this->diamond_square(this->width, this->height);
}

void HeightMapMesh::diamond_square(int width, int height)
{
    float *data;
    char *flag;
    glm::ivec2 p0;
    glm::ivec2 p1;
    glm::ivec2 p2;
    glm::ivec2 p3;
    int total_height = 0;

    this->height = height;
    this->width = width;

    data = (float *)malloc(sizeof(float) * width * height);
    flag = (char *)malloc(sizeof(char) * width * height);

    this->suavity = 2.0 + (((float)(rand() % 2000) / 1000.0) - 1.0);

    this->max_height = 0;
    this->min_height = 0;

    for (int y = 0; y < this->height; y++)
    {
        memset(&flag[y], 0, this->width);
    }

    p0.x = 0;
    p0.y = 0;

    p1.x = this->width - 1;
    p1.y = 0;

    p2.x = 0;
    p2.y = this->height - 1;

    p3.x = this->width - 1;
    p3.y = this->height - 1;

    int p0_coord = this->address_of_coord(p0);
    int p1_coord = this->address_of_coord(p1);
    int p2_coord = this->address_of_coord(p2);
    int p3_coord = this->address_of_coord(p3);

    data[p0_coord] = 0;
    data[p1_coord] = 0;
    data[p2_coord] = 0;
    data[p3_coord] = 0;

    this->diamond_square_recursion(p0, p1, p2, p3, 1.0, data, flag);

    total_height = this->max_height - this->min_height;
    // Normaliza os dados
    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            int coord = y * this->width + x;
            data[coord] = (data[coord] - this->min_height) / total_height;
        }
    }

    this->from_float_array(data, this->width, height);
    free(data);
    free(flag);
}

glm::vec3 HeightMapMesh::get_point_from_vb(int x, int y)
{
    int p_coord = this->address_of_coord(glm::ivec2(x, y));
    p_coord = p_coord * this->vertex_elements_count;

    return glm::vec3(
        this->vertex_buffer[p_coord],
        this->vertex_buffer[p_coord + 1],
        this->vertex_buffer[p_coord + 2]);
}

float HeightMapMesh::get_height_from_3p(float x, float y, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    glm::vec3 v1 = glm::vec3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    glm::vec3 v2 = glm::vec3(p1.x - p3.x, p1.y - p3.y, p1.z - p3.z);
    glm::vec3 n = glm::cross(v1, v2);

    float d = n.x * p1.x + n.y * p1.y + n.z * p1.z;
    return -(n.x * x + n.z * y - d) / n.y;
}

float HeightMapMesh::get_height(float x, float y) // o y deve receber o Z
{
    float nx = x + (this->width / 2.0);
    float ny = y + (this->height / 2.0);

    int min_x_i = floor(nx);
    int min_y_i = floor(ny);

    int max_x_i = min_x_i + 1;
    int max_y_i = min_y_i + 1;

    // decisão da face
    float dx = (nx - min_x_i);
    float dy = (ny - min_y_i);
    int face_i = dx + dy > 1 ? 1 : 0;

    // pontos das diagonais
    glm::vec3 p2 = this->get_point_from_vb(min_x_i, max_y_i);
    glm::vec3 p3 = this->get_point_from_vb(max_x_i, min_y_i);

    // terceiro ponto
    glm::vec3 p1;
    if (face_i == 0)
    {
        p1 = this->get_point_from_vb(min_x_i, min_y_i);
    }
    else
    {
        p1 = this->get_point_from_vb(max_x_i, max_y_i);
    }

    return this->get_height_from_3p(x, y, p1, p2, p3);
}

const std::string HeightMapMesh::height_map_folder = std::string(CMAKE_ROOT_DIR HEIGHT_MAP_MESH_DEFAULT_FOLDER);

#endif