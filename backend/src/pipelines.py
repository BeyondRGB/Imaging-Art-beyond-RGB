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
from calibration import color_calibrate


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

    # Generate array swap space and load files
    packet.generate_swap()
    packet.load_calibration_imgs()

    # Calibration pass
    preprocess(packet)
#    color_calibrate(packet)
    # TODO validate solution
    # TODO render target
    # TODO output target to file

    # TODO remove the fillowing once target output to file is done
    import cv2
    target = packet.get_target_img()
    t1 = cv2.cvtColor(target[0],  cv2.COLOR_RGB2BGR)
    print(t1)
    cv2.imwrite("out.tiff", t1)

    # TODO Batch Processing
    #   do
    #       itterate to next subject
    #       load flat and dark fields
    #       preprocess (flat and dark will be unloaded)
    #       render
    #       output
    #   while there are more subjects

    return
    import time
    while True:
        time.sleep(10000000)
