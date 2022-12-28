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


def main():
    """ App entry point """
    if len(sys.argv) != 10:
        print(len(sys.argv))
        print("Missing File Paths")
        exit()

    # directory = sys.argv[1]

    packet = Packet()

    for i in range(1, 9):
        # packet.files.append(directory + "/" + sys.argv[i])
        packet.files.append(sys.argv[i])

    # TODO error handling
    processing_pipeline(packet)


if __name__ == "__main__":
    main()
