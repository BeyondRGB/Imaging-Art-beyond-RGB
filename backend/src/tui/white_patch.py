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
row_str = ""
col_str = ""


def white_patch(stdscr):
    """ Welcome screen
    [in] stdscr : screen to draw welcome on
    [return] 0: normal finish; 1: user requested quit
    """
    global row_str, col_str

    while True:
        __draw_scr(stdscr)
        c = stdscr.getch()
        if c == ord('q'):
            return 1
        elif c == ord('c'):
            break
        else:
            __handle_input(c)

    # Subtract 1 to convert to 0 index
    return 0, (int(row_str)-1, int(col_str)-1)


def __handle_input(c: int):
    """ Handle user input
    we are only allowing numbers and movement here (whitelisting)
    [in] c : key pressed
    [post] row and column strings updated if nessesary
    """
    global row_str, col_str, current_string
    if c == curses.KEY_UP:
        current_string = (current_string - 1) % 2
    elif c == curses.KEY_DOWN or c == curses.KEY_ENTER or c == 10 or c == 13:
        # KEY_DOWN or KEY_ENTER
        current_string = (current_string + 1) % 2
    elif c >= ord('0') and c <= ord('9'):
        __append_string(chr(c))
    elif c == curses.KEY_BACKSPACE or c == '\b' or c == '\x7f':
        __backspace_string()


def __append_string(c: int):
    """ Determine active string and append to it
    [in] c : character to append
    [post] c appended to string
    """
    global row_str, col_str, current_string
    if current_string:
        col_str = col_str + c
    else:
        row_str = row_str + c


def __backspace_string():
    """ Determine active string and delete the last character
    [post] deleted last character from string
    """
    global row_str, col_str, current_string
    if current_string:
        col_str = col_str[:-1]
    else:
        row_str = row_str[:-1]


def __draw_scr(stdscr):
    """ Draw welcome message
    [in] stdscr : screen to draw on
    [post] screen updated
    """
    global row_str, col_str, current_string
    h, w = stdscr.getmaxyx()
    stdscr.clear()
    stdscr.border()

    txt = ["White Patch",
           "",
           "Here you will specify where the white patch is. Starting from the top left corner as row 1 column 1, indicate which",
           "patch you are using as the white patch. Use the arrow keys to navigage.",
           "",
           "When finished, press \"c\" to confirm selection"]

    for i, t in enumerate(txt):
        stdscr.addstr(i+1, 2, t)

    if current_string == 0:
        stdscr.addstr(9, 2, "Row: " + row_str, curses.color_pair(1))
        stdscr.addstr(10, 2, "Column: " + col_str)
    else:
        stdscr.addstr(9, 2, "Row: " + row_str)
        stdscr.addstr(10, 2, "Column: " + col_str, curses.color_pair(1))
