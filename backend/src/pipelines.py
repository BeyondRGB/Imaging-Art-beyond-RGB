""" pipelines.py
Collection of all pipelines

Functions:
    processing_pipeline(ifiles)

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
from rgbio import load_image


def processing_pipeline(ifiles):
    """ main color calibration pipeline
    [in] ifiles : list of image file location touples ([(imgA, imgB)])
    [out] TODO
    """
    target_file_a = load_image(ifiles[0])
    target_file_b = load_image(ifiles[1])
    white_file_a = load_image(ifiles[2])
    white_file_b = load_image(ifiles[3])
    dark_file_a = load_image(ifiles[4])
    dark_file_b = load_image(ifiles[5])

    print("Done")
