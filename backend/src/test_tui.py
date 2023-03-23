import unittest

from tui import sort_images


class TestSortImages(unittest.TestCase):
    def test_eight_images_preordered_standard_names(self):
        imported_images = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW", "print_A.ARW","print_B.ARW"]
        actual_output = sort_images(imported_images, True)
        expected_output = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW", "print_A.ARW","print_B.ARW"]
        self.assertEqual(actual_output, expected_output)

    def test_eight_images_preordered_nonstandard_names(self):
        imported_images = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "camera_A.ARW", "dark_B.ARW", "exhibit_A.ARW","exhibit_B.ARW"]
        actual_output = sort_images(imported_images, True)
        expected_output = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "camera_A.ARW", "dark_B.ARW", "exhibit_A.ARW","exhibit_B.ARW"]
        self.assertEqual(actual_output, expected_output)

    def test_eight_images_role_unordered_lighting_ordered_standard_names(self):
        imported_images = ["dark_A.ARW", "dark_B.ARW", "print_A.ARW","print_B.ARW", "white_A.ARW", "white_B.ARW", "targets_A.ARW", "targets_B.ARW"]
        actual_output = sort_images(imported_images, True)
        expected_output = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW", "print_A.ARW","print_B.ARW"]
        self.assertEqual(actual_output, expected_output)

    def test_eight_images_role_unordered_lighting_unordered_standard_names(self):
        imported_images = ["dark_A.ARW", "dark_B.ARW", "print_B.ARW", "print_A.ARW", "white_A.ARW", "white_B.ARW", "targets_B.ARW", "targets_A.ARW"]
        actual_output = sort_images(imported_images, True)
        expected_output = ["targets_B.ARW", "targets_A.ARW", "white_B.ARW", "white_A.ARW", "dark_B.ARW", "dark_A.ARW", "print_B.ARW","print_A.ARW"]
        print(actual_output)
        self.assertEqual(actual_output, expected_output)

    def test_six_images_preordered_standard_names(self):
        imported_images = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW"]
        actual_output = sort_images(imported_images, False)
        expected_output = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW"]
        self.assertEqual(actual_output, expected_output)

    def test_batch_images_preordered_standard_names(self):
        imported_images = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW", "print_A.ARW","print_B.ARW", "print2_A.ARW","print2_B.ARW"]
        actual_output = sort_images(imported_images, True)
        expected_output = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW", "print_A.ARW","print_B.ARW", "print2_A.ARW","print2_B.ARW"]
        self.assertEqual(actual_output, expected_output)

    def test_batch_images_role_unordered_standard_names(self):
        imported_images = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW", "print2_B.ARW","print_A.ARW","print_B.ARW", "print2_A.ARW"]
        actual_output = sort_images(imported_images, True)
        expected_output = ["targets_A.ARW", "targets_B.ARW", "white_A.ARW", "white_B.ARW", "dark_A.ARW", "dark_B.ARW", "print_A.ARW","print_B.ARW", "print2_A.ARW","print2_B.ARW"]
        print(actual_output)
        self.assertEqual(actual_output, expected_output)


if __name__ == '__main__':
    unittest.main()