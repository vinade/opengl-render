import numpy as np
import matplotlib.pyplot as plt

def preview_image_channel(img, id, offset=0, channels_first=False):

    height = img.shape[0]
    width = img.shape[1]
    channels = img.shape[2]

    if channels_first:
        channels = img.shape[0]
        height = img.shape[1]
        width = img.shape[2]

    output = 'P3\n'
    output = output + '# debug\n'
    output = output + f'{width} {height}\n'
    output = output + '255\n'
    for y in range(height - 1, - 1, - 1):
        for x in range(width):

            r = None
            g = None
            b = None

            if channels_first:
                r = img[offset + 0][y][x]
            else:
                r = img[y][x][offset + 0]

            r = r * 255.0
            r = int(r)
            r = r if r < 256 else 255
            r = r if r > 0 else 0

            if channels > 1:
                if channels_first:
                    g = img[offset + 1][y][x]
                    b = img[offset + 2][y][x]
                else:
                    g = img[y][x][offset + 1]
                    b = img[y][x][offset + 2]

                g = g * 255.0
                g = int(g)
                g = g if g < 256 else 255
                g = g if g > 0 else 0

                b = b * 255.0
                b = int(b)
                b = b if b < 256 else 255
                b = b if b > 0 else 0

                output = output + f'{r} {g} {b} '
            else:
                output = output + f'{r} {r} {r} '

        output = output + '\n'

    filename = f'./output/images/{id}.ppm'
    with open(filename, 'w') as f:
        f.write(output)


def load_from_png(path):
    im = plt.imread(path)
    im = im[:,:,:3]
    return np.array(im)


def save_to_png(id, data):
    img_data = data
    path = f'./output/images/{id}.png'
    if data.shape[2] == 1:
        img_data = data.reshape(data.shape[0], data.shape[1])
    img_data = img_data[::-1]
    plt.imsave(path, img_data, cmap='Greys_r')

