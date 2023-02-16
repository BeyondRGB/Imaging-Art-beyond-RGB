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
from rendering import render
from util import extract_camsigs


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
    packet.generate_swap()

    """ Calibration
    We need to get the calibration matrices for both color transformation and
    spectral imaging before we render all of the subjects including the target.
    See block comments for memory information
    """
    packet.load_calibration_imgs()  # flat, dark, and targets loaded
    preprocess(packet)  # flat and dark unloaded here
    packet.camsigs = extract_camsigs(packet)  # needed for both calibrations
    packet.unload_target()  # Targets no longer needed until rendering
    color_calibrate(packet)
    packet.delcamsigs()  # Cleanup

    """ Render and Save (Batch Processing)
    At this point we have the color transformation matrix and need to apply it to
    all images including the target.
    """
    # TODO finish description
    render(packet)  # Loading is internal; we don't need the images after this
    # TODO image saving




    # TODO remove the fillowing once target output to file is done
    import cv2
    t1 = cv2.cvtColor(packet.render,  cv2.COLOR_RGB2BGR)
    cv2.imwrite("out.tiff", t1)

    return
    import time
    while True:
        time.sleep(10000000)
