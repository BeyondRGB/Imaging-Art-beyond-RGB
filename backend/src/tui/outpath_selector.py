""" outpath_selector.py
Module for selecting outpath graphically

Functions:
    outpath_selector(files) : run the outpath selection pages

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import curses
import tkinter as tk


def outpath_selector(stdscr):
    """ run the outpath selection pages
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

    outpath = __gather_outpath()

    return 0, outpath


def __draw_intro(stdscr):
    """ Show page intro screen
    [in] stdscr : screen for printing
    [post] intro message on screen
    """
    stdscr.clear()
    stdscr.border()

    txt = ["Select Output Folder",
           "",
           "This step selects where you would like the results of calibration to be output. Like before you will get a dialogue.",
           "Only one item may be selected. This selection must be a folder.",
           "",
           "",
           "To continue press the ENTER key."]

    for i, t in enumerate(txt):
        stdscr.addstr(i+1, 2, t)


def __gather_outpath():
    """ Gather outpath from user
    [out] outpath
    """
    import tkinter as tk
    from tkinter import filedialog

    root = tk.Tk()
    root.withdraw()

    file_path = filedialog.askdirectory()
    return file_path
