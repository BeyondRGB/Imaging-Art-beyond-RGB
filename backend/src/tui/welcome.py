""" welcome.py
Welcome page

Functions:
    welcome(args) : run the welcome screen

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import curses


def welcome(stdscr):
    """ Welcome screen
    [in] stdscr : screen to draw welcome on
    [return] 0: normal finish; 1: user requested quit
    """
    h, w = stdscr.getmaxyx()
    if w < 120 or h < 20:
        return 2

    __draw_welcome(stdscr)
    while True:
        c = stdscr.getch()
        if c == ord('q'):
            return 1
        elif c == curses.KEY_ENTER or c == 10 or c == 13:  # ENTER pressed
            return 0
    return 0


def __draw_welcome(stdscr):
    """ Draw welcome message
    [in] stdscr : screen to draw on
    [post] screen updated
    """
    stdscr.clear()
    stdscr.border()

    txt = ["Welcome to BeyondRGB",
           "",
           "This step selects where you would like the results of calibration to be output. Like before you will get a dialogue.",
           "This is a Textual User Interface (TUI) for running BeyondRGB. In this mode, only calibration is supported. This",
           "prompt will guide you through the process of setting up a calibration run. Please read each step carefully and",
           "follow the prompts. To exit, press 'q' at any time.",
           "",
           "To continue press the ENTER key."]

    for i, t in enumerate(txt):
        stdscr.addstr(i+1, 2, t)
