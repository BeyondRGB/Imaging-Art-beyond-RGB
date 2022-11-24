""" exceptions.py
Collection of application exceptions

Exceptions:

Authors:
    Brendan Grau <https://github.com/Victoriam7>

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""


class MissingFilesException(Exception):
    """ We did not recieve enough files to run the pipeline """

    def __init__(self, wanted, found):
        """ init Exception
        [in] wanted : number of files expected
        [in] found  : number of files found
        """
        self.wanted = wanted
        self.found = found

    def __str__(self):
        """ Print exception a string"""
        return "Error: Expected %d files, found %d" % (self.wanted, self.found)
