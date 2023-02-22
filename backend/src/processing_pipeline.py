""" processing_pipeline.py
Processing pipeline functions

Functions:
    processing_pipeline(packet)

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import numpy as np

# Local imports
from exceptions import MissingFilesException
from preprocessing import preprocess
from calibration import color_calibrate
from rendering import render
from constants import TARGET_RADIUS
from packet import *


def processing_pipeline(packet):
    """ main color calibration pipeline
    [in] packet : packet to send through the pipeline
    [raise] IOError, FileNotFoundError, MissingFilesException
    [raise] ZeroDivisionError
    """
    num_files = len(packet.files)

    # Validate request
    if num_files < 6 or num_files % 2 != 0:
        raise MissingFilesException(6, num_files)

    # Setup
    generate_swap()

    """ Calibration
    We need to get the calibration matrices for both color transformation and
    spectral imaging before we render all of the subjects including the target.
    See block comments for memory information
    """
    packet.load_calibration_imgs()  # flat, dark, and targets loaded
    preprocess(packet)  # flat and dark unloaded here
    packet.camsigs = packet.extract_camsigs()  # needed for both calibrations
    packet.unload_target()  # Targets no longer needed until rendering
    color_calibrate(packet)
    pkt_camsigs_del(packet)  # Cleanup

    """ Render and Save (Batch Processing)
    At this point we have the color transformation matrix and need to apply it
    to all images including the target. First we render the target in a special
    pass and then loop over all remaining subjects rendering and saving them
    one by one.
    """
    # TODO finish description
    load_target(packet)
    render(packet)  # Subject array is deleted; we now have the final render
    # TODO image saving
    # TODO add batch

    # TODO remove the fillowing once target output to file is done
    import cv2
    print(packet.render.shape)
    t1 = cv2.cvtColor(packet.render,  cv2.COLOR_RGB2BGR)
    cv2.imwrite("out.tiff", t1)

    return
    import time
    while True:
        time.sleep(10000000)


def extract_camsigs(packet):
    """ Generate camsigs array
    [in] packet : pipeline packet
    [out] camsigs array
    """
    t_img = packet.get_target_img()
    tr = TARGET_RADIUS

    # TODO fix orientation
    camsigs = np.ndarray((6, 130))
    siglist = packet.target.gen_siglist()
    for i, sig in enumerate(siglist):
        cell = t_img[0][sig[1]-tr:sig[1]+tr, sig[0]-tr:sig[0]+tr]
        avg = np.average(cell, axis=(0, 1))
        camsigs[0, i] = avg[0]
        camsigs[1, i] = avg[1]
        camsigs[2, i] = avg[2]
        cell = t_img[1][sig[1]-tr:sig[1]+tr, sig[0]-tr:sig[0]+tr]
        avg = np.average(cell, axis=(0, 1))
        camsigs[3, i] = avg[0]
        camsigs[4, i] = avg[1]
        camsigs[5, i] = avg[2]

    return camsigs
