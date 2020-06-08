#ifndef DEPTH_INTERFACE_CPP
#define DEPTH_INTERFACE_CPP
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <stdlib.h>

#include "depth_interface.hpp"
#include "depth_sampler.hpp"

void content_creator(SampleContent *sc)
{
    depth::Sampler sampler(sc, DEPTH_MODE_SHARED_LIB);
}

extern "C" void refresh_content(SampleContent *sc)
{
    while (sc->creating)
    {
        sleep(1);
    }

    depth::Sampler::mtx_sc.lock();
    sc->request = 1;
    sc->creating = 1;
    depth::Sampler::mtx_sc.unlock();

    while (sc->creating)
    {
        sleep(1);
    }

    depth::Sampler::mtx_sc.lock();
    sc->request = 0;
    depth::Sampler::mtx_sc.unlock();
}

extern "C" SampleContent *init_content_controller(int images_n)
{
    SampleContent *sc;

    if (images_n < 1)
    {
        std::cout << "Warning: images_n inválido.\n";
        images_n = 1;
    }

    sc = (SampleContent *)malloc(sizeof(SampleContent));

    sc->content = (ImageData *)malloc(sizeof(ImageData) * images_n);
    sc->n = images_n;

    // TODO: usar controles melhores
    sc->request = 0;
    sc->creating = 0;

    srand(time(NULL));

    std::thread task_content_creator(content_creator, sc);
    task_content_creator.detach();

    return sc;
}

extern "C" void free_memory(void *p)
{
    free(p);
}

extern "C" ImageData *alloc_image(void *data)
{
    int data_size;
    int width, height;
    char channels;
    ImageData *img = (ImageData *)data;

    width = img->width;
    height = img->height;
    channels = img->channels;

    data_size = width * height;
    data_size = data_size * channels;
    data_size = data_size * sizeof(float);

    img->data = (float *)malloc(data_size);

    return img;
}

int main(int iArgc, char **cppArgv)
{

    return 0;
}

#endif