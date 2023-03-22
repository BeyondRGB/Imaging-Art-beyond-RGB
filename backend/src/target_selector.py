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
    John Akey <https://github.com/jpakey99>
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import random

import cv2
from numpy import clip
import time # can clean up as soon as adequate testing has been done

from rgbio import load_image


selecting = False # boolean if user is currently drawing the box
x_start, y_start, x_end, y_end = 0, 0, 0, 0 # box positional coordinates
corner_moving = "bottom_right"  # Options: top_left, top_right, bottom_left, bottom_right
BOX_CLICK_ERROR = 50 # the space from a corner coordinate a user can click and move that corner
color = (0,0,0) # color of the box


def __scale_img(img):
    """ Bit scale the image to 8 bit
    Copied from preprocessing.py since this is only one image and not a pair
    [in] img : image to scale
    [post] img bit scaled in place
    """
    s = ((2**8 - 1)/(2**14 - 1))  # TODO determine actual scale for each image
    img *= s  # Scale to 8 bit


def __new_end_point(x, y, corner_moving):
    """
    updates the positional variabled with updated mouse positioning
    [in] x            : the horizontal coordinate of the mouse
    [in] y            : the vertical coordinate of the mouse
    [in] corner_moving: the current corner of the rectangle the user is moving
    """
    global x_start, y_start, x_end, y_end

    if corner_moving == "bottom_right":
        x_end, y_end = x, y
    elif corner_moving == "top_left":
        x_start, y_start = x, y
    elif corner_moving == "top_right":
        x_end, y_start = x, y
    elif corner_moving == "bottom_left":
        x_start, y_end = x, y


def __mouse_select(event, x, y, flags, param):
    """ Callback for mouse control
    [in] event : the event triggering the callback
    [in] x     : mouse x position
    [in] y     : mouse y position
    [in] flags : event flags (unused)
    [in] param : even params (unused)
    """
    global selecting, x_start, y_start, x_end, y_end, corner_moving, BOX_CLICK_ERROR
    moving_redraw = 20

    if event == cv2.EVENT_LBUTTONDOWN:
        # If click is within 10 pixels of any corner move that corner (Top Left: x, y start Top Right: y start, x end Bottom Left: y end, x start, Bottom Right: x, y end)
        selecting = True
        if abs(x - x_start) <= BOX_CLICK_ERROR and abs(y - y_start) <= BOX_CLICK_ERROR:
            x_start, y_start = x, y
            corner_moving = "top_left"
        elif abs(x - x_start) <= BOX_CLICK_ERROR and abs(y - y_end) <= BOX_CLICK_ERROR:
            # bottom left
            x_start, y_end = x, y
            corner_moving = "bottom_left"
        elif abs(x - x_end) <= BOX_CLICK_ERROR and abs(y - y_end) <= BOX_CLICK_ERROR:
            # bottom right
            x_end, y_end = x, y
            corner_moving = "bottom_right"
        elif abs(x - x_end) <= BOX_CLICK_ERROR and abs(y - y_start) <= BOX_CLICK_ERROR:
            # Top right
            x_end, y_start = x, y
            corner_moving = "top_right"
        else:
            new_color()
            x_start, y_start, x_end, y_end = x, y, x, y
            corner_moving = "bottom_right"

    # Mouse is Moving
    elif event == cv2.EVENT_MOUSEMOVE:
        if selecting is True and (abs(x_start - x) > moving_redraw or abs(y_start - y) > moving_redraw or abs(y_end - y) > moving_redraw or abs(x_end - x) > moving_redraw):
            __new_end_point(x,y, corner_moving)

    # if the left mouse button was released
    elif event == cv2.EVENT_LBUTTONUP:
        # record the ending (x, y) coordinates
        __new_end_point(x,y, corner_moving)
        selecting = False  # cropping is finished


def __draw_target(img, rows, cols):
    """ Draw target on the image
    [in] img : the image to draw on
    [in] rows        : number of rows of a particular target doubled
    [in] cols        : number of columns of a particular target doubled
    [post] The image has the target drawn on it
    """
    global x_start, y_start, x_end, y_end, color

    diff = (x_end - x_start, y_end - y_start)

    cv2.rectangle(img, (x_start, y_start), (x_end, y_end), color, 10)
    for i in range(1, rows, 2):
        off_row = int(y_start + i * (diff[1] / rows))
        cv2.rectangle(img, (x_start, off_row), (x_end, off_row), color, 10)
    for i in range(1, cols, 2):
        off_col = int(x_start + i * (diff[0] / cols))
        cv2.rectangle(img, (off_col, y_start), (off_col, y_end), color, 10)


def new_color():
    """
    Generates a random RGB color when called
    """
    global color

    rand = random.Random()
    while True:
        red, green, blue = rand.randint(0, 255), rand.randint(0, 255), rand.randint(0, 255)
        if red + green + blue > 255:
            color = (red, green, blue)
            return


def select_target(target_path, rows=0, cols=0):
    """ Get target coordinates and characteristics
    Spawns a cv2 window with the image
    [in] target_path : path one of the images containing the target
    [in] rows        : number of rows of a particular target
    [in] cols        : number of columns of a particular target
    [out] xy coordinate pairs for corners of target selector ((x1,y2),(x2,y2))
    """
    global selecting, x_start, y_start, x_end, y_end

    # Generate image
    img = load_image(target_path)
    __scale_img(img)
    img = clip(img, 0, 255).astype('uint8')  # Clip any non int values

    # Setup window
    cv2.namedWindow("Target Selector", cv2.WINDOW_NORMAL)
    cv2.setMouseCallback("Target Selector", __mouse_select)
    cv2.imshow("Target Selector", img)

    color = new_color()
    # Loop until target selection confirmed
    while True:
        i = img.copy()

        if selecting:
            __draw_target(i, rows*2, cols*2)
            cv2.imshow("Target Selector", i)

        # time.sleep(0.25)

        if cv2.waitKey(1) == ord('q'):
            break

    cv2.destroyAllWindows()
    return ((x_start, y_start), (x_end, y_end))
