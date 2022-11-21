""" preprocessing.py
App init functions

Functions:
    main()

Authors:
    Brendan Grau <bsg8376@rit.edu>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import sys

# Local imports
from pipelines import processing_pipeline


def main():
    """ App entry point """
    if len(sys.argv) != 10:
        print(len(sys.argv))
        print("Missing File Paths")
        exit()

    directory = sys.argv[1]

    files = []
    for i in range(2, 10):
        print(directory + "\\" + sys.argv[i])
        files.append(directory + "/" + sys.argv[i])

    # TODO error handling
    processing_pipeline(files)


if __name__ == "__main__":
    main()
