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
import time

# Local imports
from pipelines import processing_pipeline
from packet import Packet
from target import Target
import argparse
import api


def main():
    """ App entry point """
    parser = argparse.ArgumentParser()

    # Mode can be 'api' or 'test'
    parser.add_argument('--api', action='store_true')
    parser.add_argument('top_left_x', nargs='?')
    parser.add_argument('bottom_right_x', nargs='?')
    parser.add_argument('top_left_y', nargs='?')
    parser.add_argument('bottom_right_y', nargs='?')
    parser.add_argument('white_col', nargs='?')
    parser.add_argument('white_row', nargs='?')

    """
    Images should be added in this order:
        Target A:
        Target B:
        Flat Field A:
        Flat Field B:
        Dark Field A:
        Dark Field B:
        Subject 1A:
        Subject 1B:  
    """
    parser.add_argument('images', nargs='?')
    args = parser.parse_args()

    if args.api:
        _api_accept()
    else:
        _pipeline_start(args)


def _pipeline_start(args):
    # Error checking
    if len(sys.argv) < 10:
        print(len(sys.argv))
        print("Missing File Paths")
        exit()

    # TODO: standardize parsing of arguments

    """ Setup packet """
    packet = Packet()

    # Gather target coords and white square
    top_left = (int(args.top_left_x), int(args.top_left_y))
    bottom_right = (int(args.bottom_right_x), int(args.bottom_right_y))
    target = Target(top_left, bottom_right, int(args.white_row), int(args.white_col))
    packet.target = target

    # Gather file locations
    packet.files.append(args.images)

    """ Begin pipeline """
    processing_pipeline(packet)

    # TODO error handling


def _api_accept():
    t = time.time()
    print('attempt setup')
    api.setup(True)
    print('setup complete')
    while True:
        if t + .5 < time.time():
            print("check")
            message = api.receive_message()
            if message:
                print("message received: START: \n" + message)
                print("END")
                # handle message - probably should be done in api.py
            t = time.time()



if __name__ == "__main__":
    main()
