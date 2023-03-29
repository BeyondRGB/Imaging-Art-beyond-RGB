""" white_patch.py
White patch page

Functions:
    white_patch(args) : run the white patch screen

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import curses

current_string = 0
type_string = ""
choices = ['NGT', 'APT', 'CCSG', 'CC']


def target_type(stdscr):
    """ Welcome screen
    [in] stdscr : screen to draw welcome on
    [return] 0: normal finish; 1: user requested quit
    """
    global type_string

    while True:
        __draw_scr(stdscr)
        c = stdscr.getch()
        if c == ord('q'):
            return 1
        elif c == curses.KEY_DOWN or c == curses.KEY_ENTER or c == 10 or c == 13:
            if __handle_enter():
                return 0, type_string
            else:
                type_string = ""
        else:
            __handle_input(c)
            

def __handle_enter() -> bool:
    global type_string, choices

    return type_string in choices


def __handle_input(c: int):
    """ Handle user input
    we are only allowing numbers and movement here (whitelisting)
    [in] c : key pressed
    [post] row and column strings updated if nessesary
    """
    global type_string
    __append_string(chr(c).upper())


def __append_string(c: int):
    """ Determine active string and append to it
    [in] c : character to append
    [post] c appended to string
    """
    global type_string
    if c == curses.KEY_BACKSPACE:
        type_string[-1] = ""
    else:
        type_string = type_string + c


def __draw_scr(stdscr):
    """ Draw welcome message
    [in] stdscr : screen to draw on
    [post] screen updated
    """
    global type_string
    h, w = stdscr.getmaxyx()
    stdscr.clear()
    stdscr.border()

    txt = ["Welcome to BeyondRGB",
           "",
           "This step tells calibration where the target is in the image. You will be shown a target image",
           "First, you will specify what target type you are using.",
           "The options are NGT, APT, CCSG, CC.  Custom target types are not yet supported",
           "",
           "To continue press the ENTER key."]

    for i, t in enumerate(txt):
        stdscr.addstr(i+1, 2, t)

    stdscr.addstr(9, 2, "Type: " + type_string, curses.color_pair(1))
