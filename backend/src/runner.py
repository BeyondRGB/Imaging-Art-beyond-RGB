""" runner.py
Program for running the BeyondRGB backend from the command line with a TUI.
This is not meant to replace the frontend, but is a continent way to test the
backend without needing to reenter settings or install nodejs.

Functions:
    settings()

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import npyscreen
import subprocess
from target_selector import select_target


def settings(*args):
    """ Gather settings using npyscreen
    This can only be called in an npyscreen basic wrapper
    [in] args : arguments
    [out] target coordinates
    [out] column of white space
    [out] row of white space
    [out] list of npyscreen object with values that are the file paths
    """
    # Form to get number of images
    form = npyscreen.Form(name='Number of Images')
    num_images = form.add(npyscreen.TitleText, name='Image Count:')
    form.edit()

    # TODO error check num_images

    # Form to get list of files
    # TODO use num_images to dynamically generate form
    form = npyscreen.Form(name='Select Files')
    files = []
    files.append((form.add(npyscreen.TitleFilename, name="Target A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Target B:")))
    files.append((form.add(npyscreen.TitleFilename, name="Flat Field A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Flat Field B:")))
    files.append((form.add(npyscreen.TitleFilename, name="Dark Field A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Dark Field B:")))
    files.append((form.add(npyscreen.TitleFilename, name="Subject 1A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Subject 1B:")))
    form.edit()

    # Get coordinates
    coords = select_target(files[0].value)

    # From to select white area
    form = npyscreen.Form(name='White Space')
    col = form.add(npyscreen.TitleText, name='White Column:')
    row = form.add(npyscreen.TitleText, name='White Row:')
    form.edit()

    return coords, int(col.value), int(row.value), files


if __name__ == '__main__':
    """ This is not in a main function because npyscreen had a fit when I tried
    that. Feel free to try, but this is an application for testing so it does
    not need to be made with good style
    """
    coords, col, row, files = npyscreen.wrapper_basic(settings)

    xs = int(coords[0][0])
    xe = int(coords[1][0])
    ys = int(coords[0][1])
    ye = int(coords[1][1])

    paths = ""
    for f in files:
        paths = paths + "%s " % f.value

    settings = "%d %d %d %d %d %d" % (xs, xe, ys, ye, col, row)
    args = "%s %s" % (settings, paths)

    # Allow user to run with the given setting any number of times
    while(1):
        print("Settings:")
        print("\tTarget Corners:")
        print("\t\tx_start = " + str(xs))
        print("\t\tx_end   = " + str(xe))
        print("\t\ty_start = " + str(ys))
        print("\t\ty_end   = " + str(ye))
        print("\tWhite Space:(%d, %d)" % (col, row))
        print("\tFiles:")
        for f in files:
            print("\t\t%s" % f.value)
        input("Press ENTER to run Beyond RGB with the above settings")
        print("Running...")
        print("python3 main.py " + args)
        subprocess.run(["python3", "main.py"] + args.split())
