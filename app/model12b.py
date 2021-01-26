import time
import hashlib
from depth_lib.thread_controller import Thread
from keras.layers import BatchNormalization, Reshape, Flatten, DepthwiseConv2D
from keras.layers import Activation, Dropout, Flatten, Dense, concatenate
from keras.layers import Conv2D, MaxPooling2D, UpSampling2D, Input
from keras.layers import Conv3D
from keras.models import Sequential, model_from_json, Model
from keras.preprocessing.image import ImageDataGenerator, array_to_img, img_to_array, load_img
from keras import backend as K
import tensorflow as tf
import numpy as np
import keras
import depth_lib.image_utils as image_utils
from depth_lib.data_loader import DepthDataLoader
import random
import gc
from cv2 import cv2

FILTERS = 64
HALF_FILTERS = FILTERS // 2
DOUBLE_FILTERS = FILTERS * 2


class Kernel:

    K1 = (1, 1)
    K3 = (3, 3)
    K5 = (5, 5)
    K7 = (7, 7)


class DepthModel:

    folder = './models'

    def __init__(self, model_name=None):
        ''' construtor '''
        filepath = f'{DepthModel.folder}/{model_name}'
        try:
            f = open(filepath)
            f.close()
            self.load(model_name)
            return
        except FileNotFoundError:
            pass
        self.create_model((240, 320, 6), model_name)

    def load(self, model_name):
        self.model_name = model_name
        filepath = f'{DepthModel.folder}/{model_name}'
        self.model = keras.models.load_model(filepath)
        self.model.build((None, IMG_HEIGHT, IMG_WIDTH, 6))
        self.model.summary()
        print(f'Modelo carregado: {self.model_name}')

    def save(self):
        filepath = f'{DepthModel.folder}/{self.model_name}'
        self.model.save(filepath)

    def conv_block(self, block_input, filters, kernel_size, items):
        ''' Cria blocos de conv com mesmo kernel e número de filtros '''
        item_input = block_input
        for i in range(items):
            item = Conv2D(filters, kernel_size, padding="same")(item_input)
            item = Activation("relu")(item)
            item_input = item

        # return Model(inputs=block_input, outputs=item_input)
        return item_input

    def depth_conv_block(self, block_input, filters, kernel_size, items):
        ''' Cria blocos de depth conv com mesmo kernel e número de filtros '''
        item_input = block_input
        for i in range(items):
            item = DepthwiseConv2D(
                kernel_size, padding="same", depth_multiplier=1)(item_input)
            item = Conv2D(filters, Kernel.K1)(item)
            item = Activation("relu")(item)
            item_input = item

        # return Model(inputs=block_input, outputs=item_input)
        return item_input

    def conv1_conv_block(self, block_input, prefilters, filters, kernel_size, items):
        ''' Cria blocos de depth conv com mesmo kernel e número de filtros '''
        item_input = block_input
        for i in range(items):
            item = Conv2D(prefilters, Kernel.K1, padding="same")(item_input)
            item = Conv2D(filters, kernel_size)(item)
            item = Activation("relu")(item)
            item_input = item

        # return Model(inputs=block_input, outputs=item_input)
        return item_input

    def create_pre_filter(self, block_input):
        layers = []
        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            strides=2,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            strides=2,
            padding="same"))
        layers.append(Activation("relu"))

        item_input = block_input
        for layer in layers:
            item_input = layer(item_input)

        # return Model(inputs=block_input, outputs=item_input)
        return item_input

    def create_post_filter(self, block_input):
        layers = []
        # layers.append(UpSampling2D(interpolation='bilinear'))
        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            padding="same"))

        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            padding="same"))

        layers.append(DepthwiseConv2D(
            Kernel.K3,
            padding="same"))
        layers.append(Conv2D(1, Kernel.K1))
        layers.append(Activation("relu"))

        item_input = block_input
        for layer in layers:
            item_input = layer(item_input)

        # return Model(inputs=block_input, outputs=item_input)
        return item_input

    def create_middle_filter1(self, block_input):
        layers = []
        layers.append(MaxPooling2D())
        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(MaxPooling2D())
        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(Conv2D(
            DOUBLE_FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(Conv2D(
            FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(UpSampling2D(interpolation='bilinear'))
        layers.append(Conv2D(
            FILTERS,
            Kernel.K1,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(UpSampling2D(interpolation='bilinear'))
        layers.append(Conv2D(
            FILTERS,
            Kernel.K1,
            padding="same"))
        layers.append(Activation("relu"))

        item_input = block_input
        for layer in layers:
            item_input = layer(item_input)

        # return Model(inputs=block_input, outputs=item_input)
        return item_input

    def create_middle_filter2(self, block_input):
        layers = []
        layers.append(Conv2D(
            HALF_FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(Conv2D(
            HALF_FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(Conv2D(
            HALF_FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        layers.append(Conv2D(
            HALF_FILTERS,
            Kernel.K3,
            padding="same"))
        layers.append(Activation("relu"))

        item_input = block_input
        for layer in layers:
            item_input = layer(item_input)

        # return Model(inputs=block_input, outputs=item_input)
        return item_input

    def create_model(self, input_shape=None, model_name=None):
        ''' Inicialização do modelo '''
        model_input = Input(input_shape)
        pre_filter_model = self.create_pre_filter(model_input)
        middle_filter1_model = self.create_middle_filter1(
            pre_filter_model)
        middle_filter2_model = self.create_middle_filter2(
            pre_filter_model)
        middle_filter = concatenate(
            [middle_filter1_model, middle_filter2_model])
        post_filter_model = self.create_post_filter(middle_filter)

        model = Model(inputs=model_input, outputs=post_filter_model)

        if model_name is None:
            model_description = model.to_json()
            md5hash = hashlib.md5()
            md5hash.update(model_description.encode('utf8'))
            hash_id = md5hash.hexdigest()
            timestamp = str(int(time.time()))
            hash_id = f'{hash_id}_{timestamp}'
            model_name = hash_id

        self.model_name = model_name

        try:
            filepath = f'{DepthModel.folder}/{self.model_name}'
            f = open(filepath)
            f.close()
            self.load(self.model_name)
            return
        except FileNotFoundError:
            pass

        model.compile(optimizer='adadelta', loss='mse')
        model.build((None, IMG_HEIGHT, IMG_WIDTH, 6))
        model.summary()
        self.model = model

        print(f'Modelo criado: {self.model_name}')

    def train(self, input_data, output_data):
        ''' Treinamento '''
        return self.model.train_on_batch(
            input_data,
            output_data,
            reset_metrics=False)

    def evaluate(self, validation_data):
        ''' Validação '''
        acc_total = 0
        for i in range(validation_data['input'].shape[0]):
            acc = self.model.evaluate(
                validation_data['input'][i], validation_data['output'][i], verbose=0)
            acc_total = acc_total + acc
        return acc_total / float(validation_data['input'].shape[0])

    def predict(self, input_data):
        output = self.model.predict(input_data)
        output = cv2.resize(
            output[0],
            (IMG_WIDTH, IMG_HEIGHT),
            interpolation=cv2.INTER_CUBIC)
        output = output[..., np.newaxis]
        return output


IMG_WIDTH = 320
IMG_HEIGHT = 240
IMAGES_N = 25
BATCH_SIZE = 5

preview_n = 4
# preview_n = BATCH_SIZE if preview_n > BATCH_SIZE else preview_n
version = '0.0.3'
model_name = f'model12_{version}'

K.clear_session()
tf.random.set_seed(0)
np.random.seed(0)


# VALIDATION DATA PREVIEW


def preview_img(data, img_id, offset=0):
    image_utils.preview_image_channel(
        data, f'{model_name}_{img_id}', offset)


def preview_img_png(data, img_id):
    image_utils.save_to_png(f'{model_name}_{img_id}', data)


def preview_gabarito():
    for i in range(preview_n):
        preview_img(validation_data['input'][i][0], f'gabarito_{i}__a')
        preview_img(validation_data['input'][i][0],
                    f'gabarito_{i}__c', offset=3)

        output = cv2.resize(
            validation_data['output'][i][0], (IMG_WIDTH, IMG_HEIGHT), interpolation=cv2.INTER_CUBIC)
        output = output[..., np.newaxis]
        preview_img(output, f'gabarito_{i}__b')


def fetch_data(data_obj=None):
    result = data_loader.load(BATCH_SIZE)
    if data_obj is not None:
        data_obj['input'] = result['input']
        data_obj['output'] = result['output']

    return result


# print(model.output_shape)
# exit()

# data_loader = DepthDataLoader(IMAGES_N, (160, 120))
data_loader = DepthDataLoader(IMAGES_N, (80, 60))
# data_loader = DepthDataLoader(IMAGES_N, (20, 15))
# data_loader = DepthDataLoader(IMAGES_N)
validation_data = data_loader.load(BATCH_SIZE)
preview_gabarito()
print(K.image_data_format())
print(validation_data['input'].shape)
print('done.')

# ******************************************************************************************************
model = DepthModel(model_name)

training_schedule = []
limit_ts = len(training_schedule)

training_data = fetch_data()
input_data = training_data['input']
output_data = training_data['output']
fetch_thread = None

cycle = 0
for j in range(1000000):

    if fetch_thread is not None:
        input_data = fetch_thread_response['input']
        output_data = fetch_thread_response['output']

    fetch_thread_response = {}
    fetch_thread = Thread(
        fetch_data, fetch_thread_response, thread_condition=False)
    fetch_thread.start()

    m = 1
    # if j < 100:
    #     m = 100 - j

    for k in range(m):

        if m > 10:
            if k % 10 == 0:
                print(k)

        losses = []
        for i in range(input_data.shape[0]):
            batch_input_data = input_data[i]
            batch_output_data = output_data[i]

            loss = model.train(batch_input_data, batch_output_data)
            losses.append({'index': i, 'loss': loss})

        losses.sort(key=lambda x: x['loss'], reverse=True)
        # print(f'\tmelhor:{losses[-1]["index"]} \t{losses[-1]["loss"]}')
        # print(f'\tpior:{losses[0]["index"]} \t{losses[0]["loss"]}')

        losses = losses[0:len(losses) // 2]

        for i in range(1):
            for loss_data in losses:
                batch_input_data = input_data[loss_data['index']]
                batch_output_data = output_data[loss_data['index']]
                model.train(batch_input_data, batch_output_data)

        if (not cycle % 100):

            for i in range(preview_n):
                output = model.predict(validation_data['input'][i])
                preview_img_png(output, f'validation_{i}_{cycle}')

            model.save()

        cycle = cycle + 1

    acc_mean = model.evaluate(validation_data)
    print(f'Train {j}: {acc_mean}')

    fetch_thread.join()

    del input_data
    del output_data
    gc.collect()
