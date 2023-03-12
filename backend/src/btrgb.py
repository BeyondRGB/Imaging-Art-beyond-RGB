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
from lab_refs import LAB_REF

class Btrgb:
    """
    Class representing values to be output to btrgb file.

    Members:
        output_files             : List of files to be outputted
        calibration_results      : Dict of calibration values such as DeltaE values
        verification_results     : Dict of verification values

    Methods: 
        set_output_files         : Set output files
        set_calibration_results  : Set calibration results
        set_verification_results : Set verification results
        to_json                  : Helper function returning a json object representing a btrgb class
        write_to_file            : Write btrgb class to a file in json format
    """

    output_files = {}
    calibration_results = {}
    verification_results = {}
    general_info = {}

    def __init__(self):
        self.calibration_results = {
            "double_values": [],
            "matrix_values": []
        }
        self.create_matrix_value(LAB_REF[0], "CM L*_ref")
        self.create_matrix_value(LAB_REF[1], "CM a*_ref")
        self.create_matrix_value(LAB_REF[2], "CM b*_ref")

    def create_matrix_value(self, matrix, name):
        matrix_value = {
            "rows": 10,
            "cols": 13,
            "data": list(matrix),
            "name": name
        }
        self.calibration_results["matrix_values"].append(matrix_value)

    def create_double_value(self, double, name):
        double_value = {
            "data": double,
            "name": name
        }
        self.calibration_results["double_values"].append(double_value)
        
    def create_output_file(self, file, name):
        self.output_files[name]= file

    def write_to_file(self, id):
        contents = json.dumps(self, default=lambda obj: obj.__dict__,
                        sort_keys=True, indent=4)
        with open(f"BeyondRGB_{id}.btrgb", "w", encoding='utf-8') as f:
            f.write(contents)


btrgb_file = Btrgb()