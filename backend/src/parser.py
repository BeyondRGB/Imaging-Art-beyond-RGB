import sys
import argparse


class Parser(argparse.ArgumentParser):
    """ Custom parser to allow for a custom error message """
    def error(self, message):
        self.print_help()
        sys.exit(2)
