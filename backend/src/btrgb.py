""" btrgb.py
Generating .btrgb output files.

Functions:

Authors:
    Sungmin Kim

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import json

class btrgb:
    """
    Class representing values to be output to btrgb file.

    Members:
        output_files             : List of files to be outputted
        calibration_results      : Dict of calibration values such as DeltaE values
        verification_results     : Dict of verification values
        general_info             : Dict of misc values about raw photo and target

    Methods: 
        set_output_files         : Set output files
        set_calibration_results  : Set calibration results
        set_verification_results : Set verification results
        set_general_info         : Set general info
        to_json                  : Helper function returning a json object representing a btrgb class
        write_to_file            : Write btrgb class to a file in json format
    """

    output_files = {}
    calibration_results = {}
    verification_results = {}
    general_info = {}

    def set_output_files(self, output_files):
        self.output_files = output_files

    def set_calibration_results(self, double_values,
                                integer_values, matrix_values, string_values):
        self.calibration_results.DOUBLE_VALUES = double_values
        self.calibration_results.INTEGER_VALUES = integer_values
        self.calibration_results.MATRIX_VALUES = matrix_values
        self.calibration_results.STRING_VALUES = string_values

    def set_verification_results(self, double_values,
                                integer_values, matrix_values, string_values):
        self.verification_results.DOUBLE_VALUES = double_values
        self.verification_results.INTEGER_VALUES = integer_values
        self.verification_results.MATRIX_VALUES = matrix_values
        self.verification_results.STRING_VALUES = string_values

    def set_general_info(self, double_values,
                        integer_values, matrix_values, string_values):
        self.general_info.DOUBLE_VALUES = double_values
        self.general_info.INTEGER_VALUES = integer_values
        self.general_info.MATRIX_VALUES = matrix_values
        self.general_info.STRING_VALUES = string_values

    def to_json(self):
        return json.dumps(self, default=lambda obj: obj.__dict__,
                        sort_keys=True, indent=4)

    def write_to_file(self, id):
        f = open(f"BeyondRGB_{id}.btrgb", "w")
        f.write(self.to_json)


