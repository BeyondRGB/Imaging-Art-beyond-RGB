""" target_selector.py
Routine for generating the target coordinates from a target image. This routine
prompts the user to select the target using an cv2 window in which a selector
can be dragged over the image creating a selection area.

Functions:
    __scale_img()
    __mouse_select()
    __draw_target()
    select_target()

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import cv2
import curses
from numpy import clip

from utils.rgbio import load_image


selecting = False
coords = [0, 0, 0, 0]  # xi, yi, xf, yf


def __scale_img(img):
    """ Bit scale the image to 8 bit
    Copied from preprocessing.py since this is only one image and not a pair
    [in] img : image to scale
    [post] img bit scaled in place
    """
    s = ((2**8 - 1)/(2**14 - 1))  # TODO determine actual scale for each image
    img *= s  # Scale to 8 bit


def __mouse_select(event, x, y, flags, param):
    """ Callback for mouse control
    [in] event : the event triggering the callback
    [in] x     : mouse x position
    [in] y     : mouse y position
    [in] flags : event flags (unused)
    [in] param : even params (unused)
    """
    global selecting, coords

    if event == cv2.EVENT_LBUTTONDOWN:
        coords[0], coords[1] = x, y
        coords[2], coords[3] = x, y
        selecting = True

    # Mouse is Moving
    elif event == cv2.EVENT_MOUSEMOVE:
        if selecting is True:
            coords[2], coords[3] = x, y

    # if the left mouse button was released
    elif event == cv2.EVENT_LBUTTONUP:
        # record the ending (x, y) coordinates
        coords[2], coords[3] = x, y
        selecting = False  # cropping is finished


def __draw_target(img):
    """ Draw target on the image
    [in] img : the image to draw on
    [post] The image has the target drawn on it
    """
    global coords
    xi, yi, xf, yf = coords

    color = (255, 0, 0)
    diff = (xf - xi, yf - yi)

    cv2.rectangle(img, (xi, yi), (xf, yf), color, 10)
    for i in range(1, 20, 2):
        off_row = int(yi + i * (diff[1] / 20))
        cv2.rectangle(img, (xi, off_row), (xf, off_row), color, 10)
    for i in range(1, 26, 2):
        off_col = int(xi + i * (diff[0] / 26))
        cv2.rectangle(img, (off_col, yi), (off_col, yf), color, 10)


def __select_target(target_path):
    """ Get target coordinates and characteristics
    Spawns a cv2 window with the image
    [in] target_path : path of one of the images containing the target
    [out] xy coordinate pairs for corners of target selector ((x1,y2),(x2,y2))
    """
    global selecting, coords

    # Generate image
    img = load_image(target_path)
    __scale_img(img)
    img = clip(img, 0, 255).astype('uint8')  # Clip any non int values

    # Setup window
    cv2.namedWindow("Target Selector", cv2.WINDOW_NORMAL)
    cv2.setMouseCallback("Target Selector", __mouse_select)
    cv2.imshow("Target Selector", img)

    # Loop until target selection confirmed
    while True:
        i = img.copy()

        if selecting:
            __draw_target(i)
            cv2.imshow("Target Selector", i)

        c = cv2.waitKey(1)
        if c == ord('q'):
            cv2.destroyAllWindows()
            return 1, None
        elif c == ord('c'):
            break

    cv2.destroyWindow("Target Selector")
    cv2.destroyAllWindows()
    return 0, ((coords[0], coords[1]), (coords[2], coords[3]))


def target_selector(stdscr, target_path: str):
    """ Runner for target selection
    [in] stdscr      : screen for printing
    [in] target_path : path of one of the images containing the target
    [out] rc, target coordinates
    """
    __draw_intro(stdscr)
    while True:
        c = stdscr.getch()
        if c == ord('q'):
            return 1
        elif c == curses.KEY_ENTER or c == 10 or c == 13:  # ENTER pressed
            break

    return __select_target(target_path)


def __draw_intro(stdscr):
    """ Show page intro screen
    [in] stdscr : screen for printing
    [post] intro message on screen
    """
    stdscr.clear()
    stdscr.border()

    txt = ["Select Target",
           "",
           "This step tells calibration where the target is in the image. You will be shown a target image in one of the lighting",
           "conditions. Click and hold you left mouse button down on the top left corner of the target. While holding the button",
           "down. Drag your mouse to the bottom right corner of the target. You will see grid lines withing the area selected.",
           "The intersection of the grid points should roughly line up with the center of each of the target patches. If they do",
           "not aligned, you can retry as many times as needed. Only the most recent selection will be saved. Once everything is",
           "aligned, press \"c\" to proceed to the next step.",
           "",
           "In a following step you will be asked to identify the white patch. Make note of its location now. You will provide",
           "its location by specifying which column and row it is in starting from the top left with row=1, col=1.",
           "",
           "To continue press the ENTER key."]

    for i, t in enumerate(txt):
        stdscr.addstr(i+1, 2, t)
