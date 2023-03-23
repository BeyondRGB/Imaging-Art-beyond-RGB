""" tui.py
Text user interface for the new backend.

Functions:
    run_processing_image_interface : main function to be called to start up the interface

Authors:
    John Akey <https://github.com/jpakey99>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import tkinter as tk
from tkinter import filedialog
from packet import genpacket, gentarget, Target, Packet
from target_selector import select_target
from pipeline import processing_pipeline
from difflib import SequenceMatcher


def max_probability(image_probabilities:list, role:str) -> dict:
    """
    Finds the image that has the best similarity for a given role
    [in] image_probabilities : a list of dictionaries that contain ratios for how simular each image is to a role
    [in] role                : the role currently looking at
    :[out] current_max_image : the dictionary that has the highest similarity for the role
    """
    current_max = 0
    current_max_image = []
    for image in image_probabilities:
        value = image[role]
        if value > current_max:
            current_max = value
            current_max_image = image
    return current_max_image


def compare_two_strings(first:str, second:str, example:str="") -> float or bool:
    """
    Compares two strings to which more closely matches a certain term (like Target)
    or compares the two string against an example string for a lighting condition
    [in] first        : a string representing a file name
    [in] second       : a string representing a file name or example name for role
    [in] example      : a string representing an example file name under a lighting condition
    [out] match_ratio : a ratio of how simular the first string is to the second string ~OR~
                        a boolean TRUE if the first string is more simular the example lighting condition
    """
    if example == "":
        return SequenceMatcher(None, first, second).ratio()
    else:
        return SequenceMatcher(None, first, example).ratio() >= SequenceMatcher(None, example, second).ratio()


def sort_images(images: list, artwork:bool) -> list:
    """
    Sorts images based on Role (Target, Flatfield, Darkfield, Artwork) and lighting condition.
    Lighting Condition does not matter as long as each "A" condition is at the same position relative to "B" condition
    [in] images         : a list of strings that represent the path to the images to be used
    [in] artwork        : boolean that represents if the target and artwork images are separate or not.  True: artwork and target are separate
    [out] sorted_images : images sorted based on role and lighting condition
    """
    matchingStandards = [
            ["image", "print", "object", "art", "exhibit","paint"], # art
        ["target", "color", "grid", "map", "passport" ], # target
        ["flat", "white", "ff", "flatfield", "vignetting", "vignette", "light", "correction"], # flat field
        ["dark", "black", "darkfield", "current", "signal", "internal", "camera", "correction" ] # dark field
    ]
    probabilityScoreProperties = ['artObjectProbability', 'targetProbability', 'flatFieldProbability', 'darkFieldProbability']
    image_probability = []
    for image in images:
        image_dict = {"name": image}
        shortened_name = image.split("/")[-1]
        for property in probabilityScoreProperties:
            image_dict[property] = 0

        for i in range(len(matchingStandards)):
            for string in matchingStandards[i]:
                string.lower()
                shortened_name.lower()
                image_dict[probabilityScoreProperties[i]] += compare_two_strings(string, shortened_name)
        image_probability.append(image_dict)

    sorted_images = []
    # target
    best_target_image = max_probability(image_probability, probabilityScoreProperties[1])
    sorted_images.append(best_target_image["name"])
    image_probability.remove(best_target_image)
    best_target_image = max_probability(image_probability, probabilityScoreProperties[1])
    sorted_images.append(best_target_image["name"])
    image_probability.remove(best_target_image)

    # flat field
    best_flatfield_image = max_probability(image_probability, probabilityScoreProperties[2])
    sorted_images.append(best_flatfield_image["name"])
    image_probability.remove(best_flatfield_image)
    best_flatfield_image = max_probability(image_probability, probabilityScoreProperties[2])
    sorted_images.append(best_flatfield_image["name"])
    image_probability.remove(best_flatfield_image)

    # dark field
    best_darkfield_image = max_probability(image_probability, probabilityScoreProperties[3])
    sorted_images.append(best_darkfield_image["name"])
    image_probability.remove(best_darkfield_image)
    best_darkfield_image = max_probability(image_probability, probabilityScoreProperties[3])
    sorted_images.append(best_darkfield_image["name"])
    image_probability.remove(best_darkfield_image)

    # art work
    if artwork:
        for i in range(len(image_probability)):
            best_artwork_image = max_probability(image_probability, probabilityScoreProperties[0])
            sorted_images.append(best_artwork_image["name"])
            image_probability.remove(best_artwork_image)

    example_image_a = sorted_images[0]
    for i in range(0,len(sorted_images),2):
        if not compare_two_strings(sorted_images[i], sorted_images[i+1], example_image_a):
            hold_image = sorted_images[i]
            sorted_images[i] = sorted_images[i+1]
            sorted_images[i+1] = hold_image

    return sorted_images


def select_files() -> list:
    """
    Prompts the user to input images
    [out] sorted_images : inputted images from the user that are sorted by lighting and role
    """
    print("Please select your 2 target, 2 flatfield, 2 darkfield, and optionally 2 artwork images")

    root = tk.Tk()
    root.withdraw()

    file_paths = filedialog.askopenfilenames()
    artwork_image = len(file_paths) > 6
    images = []
    for file in file_paths:
        images.append(file)

    return sort_images(images, artwork_image)


def select_output() -> str:
    """
    Prompting the user to select a directory where BryondRGB will store the processed image and output files
    [out] directory : a string that represents the location the user wants to store the output at
    """
    print("Please select the folder you want to save the processed image to")

    root = tk.Tk()
    root.withdraw()
    return filedialog.askdirectory()


def gather_target(target: str) -> Target:
    """
    [in] target     : a string containing the path to the target image
    [out] target    : an object of class Target
    """
    whitepatch_column = input("Please enter the column location of the white patch: ")
    whitepatch_row = input("Please enter the row location of the white patch: ")

    choices = {'NGT':{"rows": 10, "cols": 13, "id":0}, 'APT':{"rows": 4, "cols": 6, "id":1}, 'CCSG':{"rows": 10, "cols": 14, "id":2}, 'CC':{"rows": 4, "cols": 6, "id":3}}
    target_type_input = ""
    while target_type_input not in choices.keys():
        target_type_input = input("What is the target type? Options: NGT, APT, CCSG, CC. ").upper()
    target_type = choices[target_type_input]

    print("Please click and drag your mouse from the top-left corner to the bottom-right corner of the target")
    top_left, bottom_right = select_target(target, target_type["rows"], target_type["cols"])

    return gentarget((top_left, bottom_right),
                       (int(whitepatch_column), int(whitepatch_row)),
                       target_type["id"])


def run_processing_image_interface() -> Packet:
    """
    Gathers information from user that is needed to process an image
    [out] packet : class holding pipeline data
    """
    images = select_files()
    outpath = select_output()
    target = gather_target(images[1])
    packet = genpacket(images, target, outpath)
    return packet


if __name__ == '__main__':
    images = select_files()
    outpath = select_output()
    target = gather_target(images[1])
    packet = genpacket(images, target, outpath)
    processing_pipeline(packet)
