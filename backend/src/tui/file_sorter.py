import curses
from dataclasses import dataclass

RESERVED_LINES = 4  # Lines taken up by UI
VERT_OFFSET = 3  # Vertical offset for printing column data

@dataclass
class __Tui:
    """ Struct related to tui operations
    Members:
        idxs        : list of selected indices in each column
        col_idx     : currently selected column
        max_idxs    : list of maximum indices in each column
        max_col_idx : maximum number of columns
        col_data    : data for each column (each idx represents 1 column)
        scroll_idxs : current scrolling indicies for each column
    """
    idxs: list
    col_idx: int
    max_idxs: list
    max_col_idx: int
    col_data: list
    scroll_idxs: list


def tui(args):
    files = ['target1', 'target2', 'white1', 'white2', 'dark1', 'dark2', 'art1',
             'art2', 'art3', 'art4', 'art5', 'art6', 'art7', 'art8', 'art9',
             'art10', 'art11', 'art12', 'art13', 'art14', 'art15', 'art16']
    selects = [('Target A', ''), ('Target B', ''), ('Flat Field A', ''),
               ('Flat Field B', ''), ('Dark Field A', ''), ('Dark Field B', ''), 
               ('Art 1A', ''), ('Art 1B', ''), ('Art 2A', ''), ('Art 2B', ''),
               ('Art 3A', ''), ('Art 3B', ''), ('Art 4A', ''), ('Art 4B', ''),
               ('Art 5A', ''), ('Art 5B', ''), ('Art 6A', ''), ('Art 6B', ''),
               ('Art 7A', ''), ('Art 7B', ''), ('Art 8A', ''), ('Art 8B', '')]

    # Init
    stdscr = __init_curses()
    tui = __Tui([0, 0], 0, [len(files), len(selects)],
                2, [files, selects], [0, 0])

    # Run
    try:
        __run_sorter(stdscr, tui)
    except Exception:
        print('Tui error')
    finally:
        __cleanup_curses(stdscr)


def __run_sorter(stdscr, tui: __Tui):
    __draw_sorter(stdscr, tui)
    while True:
        c = stdscr.getch()
        if c == ord('q'):
            break
        else:
            __keypress(c, tui)

        __draw_sorter(stdscr, tui)


def __keypress(c: int, tui: __Tui):
    if c == curses.KEY_DOWN:
        col_idx = tui.col_idx
        if tui.max_idxs[col_idx] > 0:
            tui.idxs[col_idx] = (tui.idxs[col_idx] + 1) % tui.max_idxs[col_idx]
    elif c == curses.KEY_UP:
        col_idx = tui.col_idx
        if tui.max_idxs[col_idx] > 0:
            tui.idxs[col_idx] = (tui.idxs[col_idx] - 1) % tui.max_idxs[col_idx]
    elif c == curses.KEY_RIGHT:
        tui.col_idx = (tui.col_idx + 1) % tui.max_col_idx
    elif c == curses.KEY_LEFT:
        tui.col_idx = (tui.col_idx - 1) % tui.max_col_idx
        tui.idxs[1] = __reset_right_idx(tui)  # move idx to first empty
    elif c == curses.KEY_ENTER or c == 10 or c == 13:  # ENTER pressed
        __keypress_enter(tui)


def __keypress_enter(tui: __Tui):
    if tui.col_idx == 0:
        """ Enter pressed in left column
        After ensuring we have data to work with, we remove the entry from the
        left column and add it to the current index of the right column. We
        must then update all indices including maximums checking for out of
        bounds issues.
        """
        if tui.max_idxs[0] == 0:
            return  # There's nothing to press enter on
        f = tui.col_data[0].pop(tui.idxs[0])
        tui.col_data[1][tui.idxs[1]] = (tui.col_data[1][tui.idxs[1]][0], f)
        # Update indices
        at_end = tui.idxs[1] == tui.max_idxs[1] - 1
        if not at_end and tui.col_data[1][tui.idxs[1]+1][1] != '':
            # Next index is not empty; find first empty
            tui.idxs[1] = __reset_right_idx(tui)
        elif not at_end:
            tui.idxs[1] += 1
        tui.max_idxs[0] -= 1
        # Only decrement left col idx if it would become invalid otherwise
        if tui.idxs[0] >= tui.max_idxs[0]:
            tui.idxs[0] -= 1
    else:
        """ Enter pressed in right column
        Simply delete the value from the right column and append it to the left
        column list. Indices must also be updated
        """
        entry = tui.col_data[1][tui.idxs[1]]  # Get value
        if entry[1] == '':
            return  # Nothing to do for ''
        tui.col_data[1][tui.idxs[1]] = (entry[0], '')  # Del from right col
        tui.col_data[0].append(entry[1])  # Append to left col
        tui.max_idxs[0] += 1  # We added a new value
        if tui.max_idxs[0] == 1:
            # Added to empty list
            tui.idxs[0] = 0


def __draw_sorter(stdscr, tui: __Tui):
    h, w = stdscr.getmaxyx()
    max_entries = ((h - RESERVED_LINES) / 2) - 1

    stdscr.clear()

    # Setup window
    stdscr.border()
    stdscr.hline(2, 1, '-', w-2)
    stdscr.vline(1, int((w-2)/2-1), '|', h-2)
    stdscr.addstr(1, 1, 'Selected Images')
    stdscr.addstr(1, int((w-2)/2), 'Ordered Images')

    __update_scroll_idxs(stdscr, tui, 0)
    __update_scroll_idxs(stdscr, tui, 1)

    # File list
    start_idx = tui.scroll_idxs[0]
    for i, f in enumerate(tui.col_data[0]):
        if i < start_idx or i > start_idx + max_entries:
            continue
        s_x = 3
        s_y = 2*(i-start_idx)+VERT_OFFSET
        if tui.col_idx == 0 and i == tui.idxs[0]:
            stdscr.addstr(s_y, s_x, f, curses.color_pair(1))
        else:
            stdscr.addstr(s_y, s_x, f)

    # Selected list
    start_idx = tui.scroll_idxs[1]
    for i, s in enumerate(tui.col_data[1]):
        if i < start_idx or i > start_idx + max_entries:
            continue
        txt = s[0] + ': ' + s[1]
        s_x = int((w-2)/2) + 2
        s_y = 2*(i-start_idx)+VERT_OFFSET
        if tui.col_idx == 1 and i == tui.idxs[1]:
            stdscr.addstr(s_y, s_x, txt, curses.color_pair(1))
        else:
            stdscr.addstr(s_y, s_x, txt)


def __update_scroll_idxs(stdscr, tui: __Tui, col: int):
    if tui.max_idxs[col] == 0:
        return  # Nothing to do

    # Calculate number of entries we can hold
    h, w = stdscr.getmaxyx()
    max_entries = ((h - RESERVED_LINES) / 2) - 1

    cur_idx = tui.idxs[col]
    max_idx = tui.max_idxs[col]
    scroll = tui.scroll_idxs[col]

    # Calculate start_idx
    if cur_idx == 0:  # needed for wrapping
        tui.scroll_idxs[col] = 0
    elif cur_idx == max_idx-1 and max_idx > max_entries:
        tui.scroll_idxs[col] = int(max_idx-max_entries-1)
    elif scroll+max_entries-3 < cur_idx and scroll+max_entries < max_idx-1:
        tui.scroll_idxs[col] = (scroll+1) % max_idx
    elif cur_idx > 3 and tui.scroll_idxs[col]+3 > cur_idx:
        tui.scroll_idxs[col] = (scroll-1) % max_idx


def __reset_right_idx(tui: __Tui):
    for i, s in enumerate(tui.col_data[1]):
        if s[1] == '':
            return i
    return len(tui.col_data[1]) - 1


def __init_curses():
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
    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()
