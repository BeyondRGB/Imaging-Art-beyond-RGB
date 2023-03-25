""" tui.py
Runner for tui mode

Functions:
    tui(args) : run the tui

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import curses
import textwrap

# Local Imports
from tui.file_sorter import file_sorter


def tui(args: list):
    """ Run tui
    [in] args : argument list
    """
    rc = 0
    files = ['target1', 'target2', 'white1', 'white2', 'dark1', 'dark2', 'art1',
             'art2', 'art3', 'art4', 'art5', 'art6', 'art7', 'art8', 'art9',
             'art10', 'art11', 'art12', 'art13', 'art14', 'art15', 'art16']

    stdscr = __init_curses()
    try:
        __welcome(stdscr)
        file_sorter(stdscr, files)
    except Exception:
        rc += 1
    finally:
        __cleanup_curses(stdscr)

    if rc:
        print("TUI Error")


def __welcome(stdscr):
    """ Welcome screen
    [in] stdscr : screen to draw welcome on
    """
    __draw_welcome(stdscr)
    while True:
        c = stdscr.getch()
        if c == ord('q'):
            __cleanup_curses(stdscr)
            exit(0)
        elif c == curses.KEY_ENTER or c == 10 or c == 13:  # ENTER pressed
            return


def __draw_welcome(stdscr):
    """ Draw welcome message
    [in] stdscr : screen to draw on
    [post] screen updated
    """
    h, w = stdscr.getmaxyx()
    stdscr.clear()
    stdscr.border()

    txt = ["Welcome to BeyondRGB",
           "",
           "This is a Textual User Interface(TUI) for running BeyondRGB. In",
           "this mode, only calibration is supported. This promp will guide",
           "you through the process of setting up a calibration run. Please",
           "read each step carefully and follow the prompts. To exit press",
           "'q' at any time.",
           "",
           "To continue press the ENTER key."]

    warn = ["",
            "",
            "Warning:",
            "It is recomended you use a large window size for this",
            "application. All future prompts assume a window size > 120.",
            "Please expand your terminal window."]

    if w < 120:
        txt.extend(warn)

    for i, t in enumerate(txt):
        stdscr.addstr(i+1, 2, t)


def __init_curses():
    """ Initialize curses and set options
    [out] curses screen
    """
    # Create screen
    stdscr = curses.initscr()

    # Options
    curses.noecho()  # Don't echo keystrokes
    curses.cbreak()  # Handle keystrokes immediately
    stdscr.keypad(True)  # Enable special character handling
    stdscr.border()
    curses.curs_set(0)

    # Color pairs
    curses.start_color()
    curses.init_pair(1, curses.COLOR_BLACK, curses.COLOR_WHITE)

    return stdscr


def __cleanup_curses(stdscr):
    """ Cleanup curses
    [in] stdscr : screen to clean up
    [post] curses objects cleaned up
    """
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()
