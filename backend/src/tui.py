import curses
from dataclasses import dataclass


files = ['target1', 'target2', 'white1', 'white2', 'dark1', 'dark2', 'art1',
         'art2']
selects = [('Target A', ''), ('Target B', ''), ('Flat Field A', ''),
           ('Flat Field B', ''), ('Dark Field A', ''), ('Dark Field B', ''),
           ('Art A', ''), ('Art B', '')]


@dataclass
class __Tui:
    """ Struct related to tui opperations
    Members:
        idxs        : list of selected indices in each column
        col_idx     : currently selected column
        max_idxs    : list of maximum indices in each column
        max_col_idx : maximum number of columns
        col_data    : data for each column (each idx represents 1 column)
    """
    idxs: list
    col_idx: int
    max_idxs: list
    max_col_idx: int
    col_data: list


def tui(args):
    # TODO remove
    import random
    random.shuffle(files)

    # Init
    tui = __Tui([0, 0], 0, [len(files), len(selects)], 2, [])  # 2 columns
    stdscr = __init_curses()

    try:
        __run_sorter(stdscr, tui)
    except:
        print('error')
    finally:
        __cleanup_curses(stdscr)


def __run_sorter(stdscr, tui: __Tui):
    __draw_sorter(stdscr, tui)
    while True:
        c = stdscr.getch()
        if c == ord('q'):
            break
        elif c == curses.KEY_DOWN:
            col_idx = tui.col_idx
            tui.idxs[col_idx] = (tui.idxs[col_idx] + 1) % tui.max_idxs[col_idx]
        elif c == curses.KEY_UP:
            col_idx = tui.col_idx
            tui.idxs[col_idx] = (tui.idxs[col_idx] - 1) % tui.max_idxs[col_idx]
        elif c == curses.KEY_ENTER or c == 10 or c == 13:  # ENTER pressed
            __handle_enter_sorter(tui)

        __draw_sorter(stdscr, tui)


def __handle_enter_sorter(tui: __Tui):
    f = files.pop(tui.idxs[0])
    selects[tui.idxs[1]] = selects[tui.idxs[1]][0], f
    tui.idxs[1] += 1
    tui.max_idxs[0] -= 1
    if tui.max_idxs[0] != 0 and tui.idxs[0] >= tui.max_idxs[0]:
        tui.idxs[0] -= 1


def __draw_sorter(stdscr, tui: __Tui):
    h, w = stdscr.getmaxyx()

    stdscr.clear()

    # Setup window
    stdscr.border()
    stdscr.hline(2, 1, '-', w-2)
    stdscr.vline(1, int((w-2)/2-1), '|', h-2)
    stdscr.addstr(1, 1, 'Selected Images')
    stdscr.addstr(1, int((w-2)/2), 'Ordered Images')

    # File list
    for i, f in enumerate(files):
        if tui.col_idx == 0 and i == tui.idxs[0]:
            stdscr.addstr((2*i)+3, 3, f, curses.color_pair(1))
        else:
            stdscr.addstr((2*i)+3, 3, f)

    # Selected list
    for i, s in enumerate(selects):
        txt = s[0] + ': ' + s[1]
        x = int((w-2)/2) + 2
        if tui.col_idx == 1 and i == tui.idxs[1]:
            stdscr.addstr((2*i)+3, x, txt, curses.color_pair(1))
        else:
            stdscr.addstr((2*i)+3, x, txt)


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
