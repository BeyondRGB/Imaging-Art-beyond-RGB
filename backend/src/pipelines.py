""" pipelines.py
Collection of all pipelines

Functions:
    processing_pipeline(files)

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import gc

# Local imports
from backend.src.preprocessing import registration
from rgbio import load_image, load_array, save_array, create_temp_file
from exceptions import MissingFilesException
from preprocessing import preprocess
from constants import IMAGE_A_IDX, IMAGE_B_IDX, WHITE_A_IDX, WHITE_B_IDX, \
    DARK_A_IDX, DARK_B_IDX, RENDERABLES_START, WHITE_SWAP_IDX, DARK_SWAP_IDX


def processing_pipeline(files):
    """ main color calibration pipeline
    Order of opperations:
        1. vaidate request
        2. load flat, dark, and target
        3. preprocess flat, dark
        4. target preprocess
        5. calibration
        6. load image pair
        7. preprocessing image
        8. render image
        9. save image
        10. repeat 6-9 until all images processed
    [in] files : list of image file location touples ([(imgA, imgB)])
    [out] TODO
    [raise] IOError, FileNotFoundError, MissingFilesException, ZeroDivisionError
    """
    imgs = []
    swap = []
    num_files = len(files)

    # Validate request
    if num_files < 8 or num_files % 2 != 0:
        raise MissingFilesException(8, num_files)

    # Generate array swap space
    swap.append(create_temp_file())
    swap.append(create_temp_file())

    # Load files for calibration
    for i in range(0, RENDERABLES_START):
        imgs.append(load_image(files[i]))

    # Target preprocessing
    preprocess(imgs)

    # Save flat and dark field arrays to disk
    save_array((imgs[WHITE_A_IDX], imgs[WHITE_B_IDX]), swap[WHITE_SWAP_IDX])
    save_array((imgs[DARK_A_IDX], imgs[DARK_B_IDX]), swap[DARK_SWAP_IDX])

    import tifffile
    import numpy as np
    s = ((2**8 - 1)/(2**16 - 1))  # TODO determine actual scale
    imgs[0][:, :, :] = imgs[IMAGE_B_IDX] * s
    # img = np.multiply(imgs[0], 255)
    img = np.clip(imgs[0], 0, 255).astype('uint8')
    tifffile.imwrite('out.tif', img, photometric='rgb')
    exit(0)
    import time
    print("Done")
    while True:
        time.sleep(10000000)

    # Delete flat and dark field arrays from memory
    # Arrays must be deleted in the reverse order they are stored in the list
    del imgs[DARK_B_IDX], imgs[DARK_A_IDX]
    del imgs[WHITE_B_IDX], imgs[WHITE_A_IDX]
    gc.collect()

    # TODO registration
    imgs = ()
    registration(imgs)
    # TODO calibration

    # Delete targets from memory
    del imgs[IMAGE_B_IDX], imgs[IMAGE_A_IDX]
    gc.collect()

    # Reset imgs list
    imgs = [None] * 6

    # Load flat and dark field arrays into memory
    imgs[WHITE_A_IDX], imgs[WHITE_B_IDX] = load_array(swap[WHITE_SWAP_IDX])
    imgs[DARK_A_IDX], imgs[DARK_B_IDX] = load_array(swap[DARK_SWAP_IDX])

    # TODO rendering

    # TODO output
