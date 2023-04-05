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

# Local Imports
from tui.welcome import welcome
from tui.file_sorter import file_sorter
from tui.white_patch import white_patch
from tui.file_selector import file_selector
from tui.target_selector import target_selector
from tui.outpath_selector import outpath_selector
from calibration.packet import genpacket, gentarget
from calibration.constants import TARGETTYPE_NGT, TARGETTYPE_APT,\
                                  TARGETTYPE_CCSG, TARGETTYPE_CC


# Target arg to TARGETTYPE translator
targ2ttype = {'NGT': TARGETTYPE_NGT,
              'APT': TARGETTYPE_APT,
              'CCSG': TARGETTYPE_CCSG,
              'CC': TARGETTYPE_CC}


def tui(args: list):
    """ Run tui
    [in] args : argument list
    [out] generated packet
    """
    # Init
    stdscr = __init_curses()

    # Run
    rc = 0
    try:
        rc = welcome(stdscr)
        __handle_rc(rc, stdscr)
        rc, files = file_selector(stdscr)
        __handle_rc(rc, stdscr)
        rc, outpath = outpath_selector(stdscr)
        __handle_rc(rc, stdscr)
        rc, files = file_sorter(stdscr, files)
        __handle_rc(rc, stdscr)
        rc, coords = target_selector(stdscr, files[0])
        __handle_rc(rc, stdscr)
        rc, white = white_patch(stdscr)
        __handle_rc(rc, stdscr)
        # TODO confirmation page
    except Exception as e:
        msg = e
        rc = -1
    finally:
        __cleanup_curses(stdscr)

    # Handle Errors
    if rc == -1:
        print("TUI Error")
        print(msg)
        exit(1)

    # Build Packet
    target = gentarget(coords, white, targ2ttype['NGT'])
    packet = genpacket(files, target)

    return packet, outpath


def __handle_rc(rc: int, stdscr):
    """ Handle return codes
    [in] rc     : return code
    [in] stdscr : screen in case we are cleaning up
    """
    if rc == 0:
        return
    elif rc == 1:
        __cleanup_curses(stdscr)
        exit(0)
    elif rc == 2:
        __cleanup_curses(stdscr)
        print("Screen size too small. Please expand your terminal")
        exit(2)


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
