""" pipelines.py
Collection of all pipelines

Functions:
    processing_pipeline(packet)

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Local imports
from exceptions import MissingFilesException
from preprocessing import preprocess


def processing_pipeline(packet):
    """ main color calibration pipeline
    Order of opperations:
        1. vaidate request
        2. load flat, dark, and target
        3. preprocess
        4. calibration
        5. load subject pair
        6. preprocessing image
        7. render subject
        8. save subject
        9. repeat 5-8 until all images processed
    [in] packet : packet to send through the pipeline
    [raise] IOError, FileNotFoundError, MissingFilesException
    [raise] ZeroDivisionError
    """
    num_files = len(packet.files)

    # Validate request
    if num_files < 8 or num_files % 2 != 0:
        raise MissingFilesException(8, num_files)

    # Generate array swap space
    packet.generate_swap()

    # Load files for calibration
    packet.load_calibration_imgs()

    # Target preprocessing
    preprocess(packet)

    import tifffile
    import numpy as np
    img = packet.imgs[0] * 255
    img = np.clip(img, 0, 255).astype('uint8')
    tifffile.imwrite('out.tif', img, photometric='rgb')
    import time
    print("Done")
    return
    while True:
        time.sleep(10000000)

    # TODO calibration

    # Load flat and dark field arrays into memory

    # TODO rendering

    # TODO output
