""" preprocessing.py
App init functions

Functions:
    main()

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import sys

# Local imports
from pipelines import processing_pipeline
from packet import Packet
from target import Target


def main():
    """ App entry point """
    # Error checking
    if len(sys.argv) < 10:
        print(len(sys.argv))
        print("Missing File Paths")
        exit()

    # TODO: standardize parsing of arguments

    """ Setup packet """
    packet = Packet()

    # Gather target coords and white square
    tl = (int(sys.argv[1]), int(sys.argv[3]))
    br = (int(sys.argv[2]), int(sys.argv[4]))
    col = int(sys.argv[5])
    row = int(sys.argv[6])
    target = Target(tl, br, row, col)
    packet.target = target

    # Gather file locations
    for i in range(7, len(sys.argv)):
        packet.files.append(sys.argv[i])

    """ Begin pipeline """
    processing_pipeline(packet)

    # TODO error handling


if __name__ == "__main__":
    main()
