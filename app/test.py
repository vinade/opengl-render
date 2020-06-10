import time
from depth_lib.data_loader import DepthDataLoader
import depth_lib.image_utils as image_utils

IMAGES_N = 20
BATCH_SIZE = 20

print('Carregando imagens...')
data_loader = DepthDataLoader(IMAGES_N)
validation_data = data_loader.load(BATCH_SIZE)
print('\tdone.')


def save_img(data, img_id, offset=0):
    image_utils.preview_image_channel(data, f'model_name_{img_id}', offset)


def save_sample():
    for i in range(20):  # apenas para teste
        save_img(validation_data['input'][0][i], f'gabarito_{i}__a')
        save_img(validation_data['output'][0][i], f'gabarito_{i}__b')
        save_img(validation_data['input'][0][i], f'gabarito_{i}__c', offset=3)


print('Salvando imagens...')
save_sample()
print('\tdone.')
