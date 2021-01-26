from cv2 import cv2
import random
import numpy as np
import ctypes


class ImageData(ctypes.Structure):
    _fields_ = [
        ('width', ctypes.c_int),
        ('height', ctypes.c_int),
        ('channels', ctypes.c_char),
        ('data', ctypes.POINTER(ctypes.c_float))
    ]

    def __init__(self, width, height, channels):
        super().__init__()
        self.width = width
        self.height = height
        self.channels = channels

    def __repr__(self):
        return 'ImageData({0}, {1}, {2})'.format(self.channels, self.height, self.width)


class SampleContent(ctypes.Structure):
    _fields_ = [
        ('n', ctypes.c_int),
        ('creating', ctypes.c_char),
        ('request', ctypes.c_char),
        ('content', ctypes.POINTER(ImageData))
    ]

    def __repr__(self):
        return 'SampleContent({0}, {1}, {2})'.format(self.n, self.creating, self.request)


class DepthDataLoader():

    def __init__(self, n=20, desired_size=None):
        self.n = n
        self.sc_pointer = DepthRender.init_content_controller(n)
        self.desired_size = desired_size

    @staticmethod
    def load_image_from_buffer(img1, img2=None, desired_size=None):
        """ Carrega imagem de um buffer organizado com channel last """

        width = img1.width
        height = img1.height
        channels = img1.channels[0]

        image = []
        for y in range(height):
            start = y * width
            line = []

            for i in range(width):
                pi = (start + i) * channels
                pixels = [img1.data[pi + ci] for ci in range(channels)]

                if img2:
                    pixels = pixels + [img2.data[pi + ci]
                                       for ci in range(channels)]

                line.append(pixels)

            image.append(line)

        if desired_size is not None:
            image = np.array(image)
            # image = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY, 1)
            # image = cv2.imdecode(image, cv2.IMREAD_GRAYSCALE)
            # image = cv2.resize(image, desired_size, interpolation=cv2.INTER_LINEAR)
            image = cv2.resize(image, desired_size,
                               interpolation=cv2.INTER_AREA)
            image = image[..., np.newaxis]

        return image

    def _load(self, batch_size=1):
        """ Carrega as imagens da shared lib """

        # estimativa do número de casos de teste (dobro do número de samples)
        n_cases = self.n
        item_cache = {}

        # Carrega os dados do render
        DepthRender.refresh_content(self.sc_pointer)
        sc = self.sc_pointer[0]
        imgs = ctypes.cast(sc.content, ctypes.POINTER(ImageData))

        input_data = []
        output_data = []
        slices = int(n_cases/batch_size)

        for i in range(slices):
            input_item = []
            output_item = []

            for j in range(batch_size):
                # sample_index = random.randrange(self.n) * 3
                sample_index = ((i * batch_size + j) % self.n) * 3

                if sample_index in item_cache:
                    # Aproveita a referência
                    input_item.append(item_cache[sample_index]['input'])
                    output_item.append(item_cache[sample_index]['output'])
                    continue

                [img_input_esq, img_output,
                    img_input_dir] = imgs[sample_index: sample_index + 3]

                input_images = DepthDataLoader.load_image_from_buffer(
                    img_input_esq, img_input_dir)
                input_item.append(input_images)

                output_images = DepthDataLoader.load_image_from_buffer(
                    img_output, desired_size=self.desired_size)
                output_item.append(output_images)

                item_cache[sample_index] = {
                    'input': input_images,
                    'output': output_images,
                }

            input_data.append(input_item)
            output_data.append(output_item)

        return {
            'input': np.array(input_data),
            'output': np.array(output_data)
        }

    def load(self, batch_size=1):
        """ Carrega as imagens da shared lib """

        return self._load(batch_size)


# Interface com o módulo DepthSampler
DepthRender = ctypes.CDLL('../build/app/libdepth-sampler.so')
DepthRender.init_content_controller.restype = ctypes.POINTER(SampleContent)
DepthRender.refresh_content.argtypes = [ctypes.POINTER(SampleContent)]
