""" file_selector.py
Module for selecting files graphically

Functions:
    file_selector(files) : run the file selection pages

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import curses
import tkinter as tk
from tkinter import filedialog


def file_selector(stdscr):
    """ run the file selection pages
    [in] stdscr : screen for printing
    [out] rc (0 normal; 1 quit), list of files
    """
    __draw_intro(stdscr)
    while True:
        c = stdscr.getch()
        if c == ord('q'):
            return 1
        elif c == curses.KEY_ENTER or c == 10 or c == 13:  # ENTER pressed
            break

    files = __gather_images()

    return 0, files


def __draw_intro(stdscr):
    """ Show page intro screen
    [in] stdscr : screen for printing
    [post] intro message on screen
    """
    stdscr.clear()
    stdscr.border()

    txt = ["Select Files",
           "",
           "This step selects the files to be processed. You will be shown a file selection dialogue. Navigate to your images and",
           "select them. All images must be in the same folder. You must select at least 3 \"A, B\" image pairs: 2 flat fields,",
           "2 dark fields, and 2 images containing a target. Targets will be returned in a calibrated form. You may add",
           "additional image pairs for bulk processing. You will match up the images in a seperate step.",
           "",
           "While in the dialogue, you can hold the 'Ctrl' key while clicking to select multiple files one at a time or hold the",
           "\"Shift\" key to select multiple files at once. Clicking and dragging may work as well depending on your computer.",
           "",
           "To continue press the ENTER key."]

    for i, t in enumerate(txt):
        stdscr.addstr(i+1, 2, t)


def __gather_images():
    """ Gather list of images from user
    [out] file list
    """
    root = tk.Tk()
    root.withdraw()
    files = list(filedialog.askopenfilenames())
    # TODO error checking
    return files
