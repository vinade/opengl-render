from cv2 import cv2
import gc
import random
from depth_lib.data_loader import DepthDataLoader
import depth_lib.image_utils as image_utils
import keras
import numpy as np
import tensorflow as tf
from keras import backend as K
from keras.preprocessing.image import ImageDataGenerator, array_to_img, img_to_array, load_img
from keras.models import Sequential, model_from_json, Model
from keras.layers import Conv2D, MaxPooling2D, UpSampling2D, Input
from keras.layers import Activation, Dropout, Flatten, Dense, concatenate
from keras.layers import BatchNormalization, Reshape, Flatten, DepthwiseConv2D
from depth_lib.thread_controller import Thread


LOAD = False

# IMG_WIDTH = 320
# IMG_HEIGHT = 240
IMG_WIDTH = 320
IMG_HEIGHT = 240
IMAGES_N = 50
BATCH_SIZE = 5

preview_n = 10
# preview_n = BATCH_SIZE if preview_n > BATCH_SIZE else preview_n
version = '0.0.25b_b'
model_name = f'model_{version}'

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


def create_model(inputShape):

    model = Sequential()

    HALF_FILTERS = 32
    FILTERS = HALF_FILTERS * 2
    DOUBLE_FILTERS = FILTERS * 2
    TETRA_FILTERS = DOUBLE_FILTERS * 2
    OCT_FILTERS = TETRA_FILTERS * 2
    HEX_FILTERS = OCT_FILTERS * 2
    KERNEL_SIZE_1 = (1, 1)
    KERNEL_SIZE_3 = (3, 3)
    KERNEL_SIZE_5 = (5, 5)
    KERNEL_SIZE_7 = (7, 7)
    KERNEL_SIZE_9 = (9, 9)

    model.add(DepthwiseConv2D(KERNEL_SIZE_3,
                              padding="same", depth_multiplier=20))
    model.add(Conv2D(FILTERS, KERNEL_SIZE_1, padding="same"))

    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(MaxPooling2D())
    model.add(Activation("relu"))

    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(MaxPooling2D())
    model.add(Activation("relu"))

    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(MaxPooling2D())
    model.add(Activation("relu"))

    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(Activation("relu"))
    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(Activation("relu"))
    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(Activation("relu"))

    # model.add(UpSampling2D(interpolation='bilinear'))  # ////////
    model.add(UpSampling2D())  # ////////
    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(Activation("relu"))

    # model.add(UpSampling2D(interpolation='bilinear'))  # ////////
    model.add(UpSampling2D())  # ////////
    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(Activation("relu"))
    model.add(Conv2D(FILTERS, KERNEL_SIZE_3, padding="same"))
    model.add(Activation("relu"))

    model.add(Conv2D(1, (3, 3), padding="same"))

    return model


def fetch_data(data_obj=None):
    result = data_loader.load(BATCH_SIZE)
    if data_obj is not None:
        data_obj['input'] = result['input']
        data_obj['output'] = result['output']

    return result


data_loader = DepthDataLoader(IMAGES_N, (160, 120))
# data_loader = DepthDataLoader(IMAGES_N)
validation_data = data_loader.load(BATCH_SIZE)
preview_gabarito()
print(K.image_data_format())
print(validation_data['input'].shape)
print('done.')

# ******************************************************************************************************

model = None

if LOAD:
    print("Carregando modelo...")
    json_file = open(f'./models/model_{model_name}.json', 'r')
    model_json = json_file.read()
    json_file.close()
    model = model_from_json(model_json)
    model.load_weights(f"./models/model_{model_name}.h5")
    print("\tmodelo carregado.")
else:
    model = create_model((IMG_HEIGHT, IMG_WIDTH, 6))

# model.compile(optimizer='rmsprop', loss='mse')
model.compile(optimizer='adadelta', loss='mse')
# model.compile(optimizer='adam', loss='msle')
model.build((None, IMG_HEIGHT, IMG_WIDTH, 6))

model.summary()
# exit()

# training_schedule = []
training_schedule = []
limit_ts = len(training_schedule)

training_data = fetch_data()
input_data = training_data['input']
output_data = training_data['output']
fetch_thread = None

for j in range(1000000):

    # if fetch_thread is not None:
    #     input_data = fetch_thread_response['input']
    #     output_data = fetch_thread_response['output']

    # fetch_thread_response = {}
    # fetch_thread = Thread(
    #     fetch_data, fetch_thread_response, thread_condition=False)
    # fetch_thread.start()

    m = 1
    if j < limit_ts:
        m = training_schedule[j]

    for k in range(m):

        if m > 10:
            if k % 10 == 0:
                print(k)
        # for k in range(10):

        losses = []
        for i in range(validation_data['input'].shape[0]):
            batch_input_data = validation_data['input'][i]
            batch_output_data = validation_data['output'][i]

            loss = model.train_on_batch(
                batch_input_data, batch_output_data, reset_metrics=False)
            losses.append({'index': i, 'loss': loss})

        losses.sort(key=lambda x: x['loss'], reverse=True)
        print(f"\tbest: {losses[-1]['loss']} \tworse: {losses[0]['loss']}")
        losses = losses[0:5]

        for i in range(2):
            for loss_data in losses:
                batch_input_data = validation_data['input'][loss_data['index']]
                batch_output_data = validation_data['output'][loss_data['index']]

                model.train_on_batch(
                    batch_input_data, batch_output_data, reset_metrics=False)

    train_acc_mean = 0
    for i in range(validation_data['input'].shape[0]):
        train_acc = model.evaluate(
            validation_data['input'][i], validation_data['output'][i], verbose=0)
        train_acc_mean = train_acc_mean + train_acc
    train_acc_mean = train_acc_mean / float(validation_data['input'].shape[0])
    print(f'Train {j}: {train_acc_mean}')

    # guarda a rede
    if not j % 100:

        mj = j
        for i in range(10):
            output = model.predict(validation_data['input'][i])
            output = cv2.resize(
                output[0], (IMG_WIDTH, IMG_HEIGHT), interpolation=cv2.INTER_CUBIC)
            output = output[..., np.newaxis]
            preview_img_png(output, f'validation_{i}_{j}')
            # preview_img(output[0], f'validation_{i}_{j}')

        model_json = model.to_json()
        with open(f"./models/model_{model_name}.json", "w") as json_file:
            json_file.write(model_json)
        # serialize weights to HDF5
        model.save_weights(f"./models/model_{model_name}.h5")

    # fetch_thread.join()

    # del input_data
    # del output_data
    # gc.collect()
