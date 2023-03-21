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
from packet import genpacket, gentarget
from target_selector import select_target
from pipeline import processing_pipeline
from difflib import SequenceMatcher


def max_probability(image_probabilities:list, role:str):
    """

    [in] image_probabilities:
    [in] role:
    :[out]:
    """
    current_max = 0
    current_max_image = []
    for image in image_probabilities:
        value = image[role]
        if value > current_max:
            current_max = value
            current_max_image = image
    return current_max_image


def compare_two_strings(first:str, second:str, example:str=""):
    if example == "":
        return SequenceMatcher(None, first, second).ratio()
    else:
        return SequenceMatcher(None, first, example).ratio() >= SequenceMatcher(None, example, second).ratio()


def find_best_match(main_string, target_strings: list):
    ratings = []
    best_match_index = 0
    for string in target_strings:
        current_rating = compare_two_strings(main_string, string)
        ratings.append({"target": string, "rating":current_rating})
        if current_rating > ratings[best_match_index][ratings]:
            best_match_index = target_strings.index(string)

    return {"ratings":ratings, "best_match":ratings[best_match_index], "best_match_index": best_match_index}


def sort_images(images: list, artwork:bool):
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

    print(sorted_images)
    return sorted_images


def select_files():
    print("Please select your 2 target, 2 flatfield, 2 darkfield, and optionally 2 artwork images")

    root = tk.Tk()
    root.withdraw()

    file_paths = filedialog.askopenfilenames()
    artwork_image = len(file_paths) > 6
    images = []
    for file in file_paths:
        images.append(file)

    return sort_images(images, artwork_image)


def select_output():
    print("Please select the folder you want to save the processed image to")

    root = tk.Tk()
    root.withdraw()
    return filedialog.askdirectory()


def gather_target(target: str):
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
    print(target_type_input)
    target_type = choices[target_type_input]
    print(target_type, target_type["id"])

    print("Please click and drag your mouse from the top-left corner to the bottom-right corner of the target")
    top_left, bottom_right = select_target(target, target_type["rows"], target_type["cols"])

    return gentarget((top_left, bottom_right),
                       (int(whitepatch_column), int(whitepatch_row)),
                       target_type["id"])


def run_processing_image_interface():
    images = select_files()
    outpath = select_output()
    target = gather_target(images[4])
    packet = genpacket(images, target, outpath)
    return packet


if __name__ == '__main__':
    images = select_files()
    outpath = select_output()
    target = gather_target(images[1])
    packet = genpacket(images, target, outpath)
    processing_pipeline(packet)
