""" file_sorter.py
Module for sorting files graphically

Functions:
    file_sorter(files) : run the file sorter pages

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python Imports
import curses
from os.path import basename
from dataclasses import dataclass

RESERVED_LINES = 4  # Lines taken up by UI
VERT_OFFSET = 3  # Vertical offset for printing column data


@dataclass
class __FileSorter:
    """ Struct related to fs operations
    Members:
        idxs        : list of selected indices in each column
        col_idx     : currently selected column
        max_idxs    : list of maximum indices in each column
        max_col_idx : maximum number of columns
        col_data    : data for each column (each idx represents 1 column)
        scroll_idxs : current scrolling indices for each column
    """
    idxs: list
    col_idx: int
    max_idxs: list
    max_col_idx: int
    col_data: list
    scroll_idxs: list


def file_sorter(stdscr, files):
    """ Main runner for the sorter
    [in] stdscr : screen to draw sorter on
    [in] files  : list of files to sort
    """
    selects = [('Target A', ''), ('Target B', ''), ('Flat Field A', ''),
               ('Flat Field B', ''), ('Dark Field A', ''), ('Dark Field B', ''), 
               ('Art 1A', ''), ('Art 1B', ''), ('Art 2A', ''), ('Art 2B', ''),
               ('Art 3A', ''), ('Art 3B', ''), ('Art 4A', ''), ('Art 4B', ''),
               ('Art 5A', ''), ('Art 5B', ''), ('Art 6A', ''), ('Art 6B', ''),
               ('Art 7A', ''), ('Art 7B', ''), ('Art 8A', ''), ('Art 8B', '')]

    # Init
    data = [len(files), len(selects)]
    fs = __FileSorter([0, 0], 0, data, 2, [files, selects], [0, 0])

    # Run
    __draw_sorter(stdscr, fs)
    while True:
        c = stdscr.getch()
        if c == ord('q'):
            return 1
        else:
            __keypress(c, fs)

        __draw_sorter(stdscr, fs)
    return 0


def __keypress(c: int, fs: __FileSorter):
    """ Handle keypresses
    [in] c : input keypress
    [in] fs : file sorter data
    [post] data in fs updated based on keypress
    """
    if c == curses.KEY_DOWN:
        col_idx = fs.col_idx
        if fs.max_idxs[col_idx] > 0:
            fs.idxs[col_idx] = (fs.idxs[col_idx] + 1) % fs.max_idxs[col_idx]
    elif c == curses.KEY_UP:
        col_idx = fs.col_idx
        if fs.max_idxs[col_idx] > 0:
            fs.idxs[col_idx] = (fs.idxs[col_idx] - 1) % fs.max_idxs[col_idx]
    elif c == curses.KEY_RIGHT:
        fs.col_idx = (fs.col_idx + 1) % fs.max_col_idx
    elif c == curses.KEY_LEFT:
        fs.col_idx = (fs.col_idx - 1) % fs.max_col_idx
        fs.idxs[1] = __reset_right_idx(fs)  # move idx to first empty
    elif c == curses.KEY_ENTER or c == 10 or c == 13:  # ENTER pressed
        __keypress_enter(fs)


def __keypress_enter(fs: __FileSorter):
    """ Handle the more complex logic of the enter key
    [in] fs : file sorter data
    [post] data in fs updated for enter keypress
    """
    if fs.col_idx == 0:
        """ Enter pressed in left column
        After ensuring we have data to work with, we remove the entry from the
        left column and add it to the current index of the right column. We
        must then update all indices including maximums checking for out of
        bounds issues.
        """
        if fs.max_idxs[0] == 0:
            return  # There's nothing to press enter on
        f = fs.col_data[0].pop(fs.idxs[0])
        fs.col_data[1][fs.idxs[1]] = (fs.col_data[1][fs.idxs[1]][0], f)
        # Update indices
        at_end = fs.idxs[1] == fs.max_idxs[1] - 1
        if not at_end and fs.col_data[1][fs.idxs[1]+1][1] != '':
            # Next index is not empty; find first empty
            fs.idxs[1] = __reset_right_idx(fs)
        elif not at_end:
            fs.idxs[1] += 1
        fs.max_idxs[0] -= 1
        # Only decrement left col idx if it would become invalid otherwise
        if fs.idxs[0] >= fs.max_idxs[0]:
            fs.idxs[0] -= 1
    else:
        """ Enter pressed in right column
        Simply delete the value from the right column and append it to the left
        column list. Indices must also be updated
        """
        entry = fs.col_data[1][fs.idxs[1]]  # Get value
        if entry[1] == '':
            return  # Nothing to do for ''
        fs.col_data[1][fs.idxs[1]] = (entry[0], '')  # Del from right col
        fs.col_data[0].append(entry[1])  # Append to left col
        fs.max_idxs[0] += 1  # We added a new value
        if fs.max_idxs[0] == 1:
            # Added to empty list
            fs.idxs[0] = 0


def __draw_sorter(stdscr, fs: __FileSorter):
    """ Draw the sorter window
    [in] stdscr : screen to draw on
    [in] fs     : file sorter data
    [post] screen updated
    """
    h, w = stdscr.getmaxyx()
    max_entries = ((h - RESERVED_LINES) / 2) - 1

    stdscr.clear()

    # Setup window
    stdscr.border()
    stdscr.hline(2, 1, '-', w-2)
    stdscr.vline(1, int((w-2)/2-1), '|', h-2)
    stdscr.addstr(1, 1, 'Selected Images')
    stdscr.addstr(1, int((w-2)/2), 'Ordered Images')

    __update_scroll_idxs(stdscr, fs, 0)
    __update_scroll_idxs(stdscr, fs, 1)

    # File list
    start_idx = fs.scroll_idxs[0]
    for i, f in enumerate(fs.col_data[0]):
        if i < start_idx or i > start_idx + max_entries:
            continue
        s_x = 3
        s_y = 2*(i-start_idx)+VERT_OFFSET
        if fs.col_idx == 0 and i == fs.idxs[0]:
            stdscr.addstr(s_y, s_x, basename(f), curses.color_pair(1))
        else:
            stdscr.addstr(s_y, s_x, basename(f))

    # Selected list
    start_idx = fs.scroll_idxs[1]
    for i, s in enumerate(fs.col_data[1]):
        if i < start_idx or i > start_idx + max_entries:
            continue
        txt = s[0] + ': ' + basename(s[1])
        s_x = int((w-2)/2) + 2
        s_y = 2*(i-start_idx)+VERT_OFFSET
        if fs.col_idx == 1 and i == fs.idxs[1]:
            stdscr.addstr(s_y, s_x, txt, curses.color_pair(1))
        else:
            stdscr.addstr(s_y, s_x, txt)


def __update_scroll_idxs(stdscr, fs: __FileSorter, col: int):
    """ Update the scroll index for a given column
    [in] stdscr : screen
    [in] fs     : file sorter data
    [in] col    : column to operate on
    [post] scroll_idxs updated
    """
    if fs.max_idxs[col] == 0:
        return  # Nothing to do

    # Calculate number of entries we can hold
    h, w = stdscr.getmaxyx()
    max_entries = ((h - RESERVED_LINES) / 2) - 1

    cur_idx = fs.idxs[col]
    max_idx = fs.max_idxs[col]
    scroll = fs.scroll_idxs[col]

    """ Calculate start_idx
    General case is to scroll up or down when within 3 of the edge. For
    wrapping to work a special case is added for the top and bottom of screen
    that causes a jump in scrolling.

    Know Bugs: Right index can go out of the screen when returning to left side
    """
    if cur_idx == 0:  # needed for wrapping
        fs.scroll_idxs[col] = 0
    elif cur_idx == max_idx-1 and max_idx > max_entries:
        fs.scroll_idxs[col] = int(max_idx-max_entries-1)
    elif scroll+max_entries-3 < cur_idx and scroll+max_entries < max_idx-1:
        fs.scroll_idxs[col] = (scroll+1) % max_idx
    elif cur_idx > 3 and fs.scroll_idxs[col]+3 > cur_idx:
        fs.scroll_idxs[col] = (scroll-1) % max_idx


def __reset_right_idx(fs: __FileSorter):
    """ Get the lowest empty index for right column
    [in] fs : file sorter data
    [out] index of lowest empty value
    """
    for i, s in enumerate(fs.col_data[1]):
        if s[1] == '':
            return i
    return len(fs.col_data[1]) - 1
