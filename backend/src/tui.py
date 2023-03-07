import tkinter as tk
from tkinter import filedialog
from packet import genpacket, gentarget
from target_selector import select_target
from pipeline import processing_pipeline


def select_files():
    root = tk.Tk()
    root.withdraw()

    file_paths = filedialog.askopenfilenames()
    images = []
    for file in file_paths:
        images.append(file)

    return images


def select_output():
    root = tk.Tk()
    root.withdraw()
    return filedialog.askdirectory()


def gather_target(target):
    """
    [in] target     : a string containing the path to the target image
    [out] target    : an object of class Target
    """
    print("Please select your 2 target, 2 flatfield, 2 darkfield, and optionally 2 artwork images")
    print("Please select the folder you want to save the processed image to")

    whitepatch_column = input("Please enter the column location of the white patch: ")
    whitepatch_row = input("Please enter the row location of the white patch: ")

    print("Please click and drag your mouse from the top-left corner to the bottom-right corner of the target")
    top_left, bottom_right = select_target(target)

    choices = ['NGT', 'APT', 'CCSG', 'CC']
    target_type_input = ""
    while target_type_input not in choices:
        target_type_input = input("What is the target type? Options: NGT, APT, CCSG, CC. ").upper()
    target_type = choices.index(target_type_input)

    return gentarget((top_left, bottom_right),
                       (int(whitepatch_column), int(whitepatch_row)),
                       target_type)


def run_processing_image_interface():
    images = select_files()
    outpath = select_output()
    target = gather_target(images[4])
    packet = genpacket(images, target, outpath)
    return packet


if __name__ == '__main__':
    images = select_files()
    outpath = select_output()
    target = gather_target(images[4])
    packet = genpacket(images, target, outpath)
    processing_pipeline(packet)
