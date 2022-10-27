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
import sys

from pipelines import processing_pipeline


def main():
    """ App entry point """
    if len(sys.argv) != 10:
        print(len(sys.argv))
        print("Missing File Paths")
        exit()

    directory = sys.argv[1]

    flist = []
    for i in range(2, 10):
        flist.append(directory + "/" + sys.argv[i])
    processing_pipeline(flist)

if __name__ == "__main__":
    main()
