""" parser.py

This parser overrides argparse to allow for automatic
printing of the help message when a user inputs invalid
arguments.

Authors:
    Keenan Miller <https://github.com/keenanm500>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import sys
import argparse


class Parser(argparse.ArgumentParser):
    """ Custom parser to allow for a custom error message """
    def error(self, message):
        self.print_help()
        sys.exit(2)
