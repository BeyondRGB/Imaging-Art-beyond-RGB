# Makefile
# This Makefile is used to build the BeyondRGB backend into an single executable
# file. 
#
# Authors:
#     Brendan Grau <https://github.com/Victoriam7>
#
# License:
#     © 2022 BeyondRGB
#     This code is licensed under the MIT license (see LICENSE.txt for details)
################################################################################
OUT_DIR = dist/
DATA_DIR = data

BUILD_ARGS_BASE = -p src --distpath $(OUT_DIR) -y --clean -F

# UNIX like systems use ':' as the separator while Windows uses ';'
BUILD_ARGS_UNIX = $(BUILD_ARGS_BASE) --add-data $(DATA_DIR):$(DATA_DIR)
BUILD_ARGS_WIN = $(BUILD_ARGS_BASE) --add-data $(DATA_DIR);$(DATA_DIR)

make: unix

unix:
	pyinstaller $(BUILD_ARGS_UNIX) src/BeyondRGB.py

win:
	pyinstaller $(BUILD_ARGS_WIN) src/BeyondRGB.py

clean:
	rm BeyondRGB.spec dist build out -rf

###### FOR TEST CASES UNZIP GITHUB EXAMPLE IMAGES IN TOP LEVEL DIRECTORY ######
test_single:
	mkdir -p out
	dist/BeyondRGB 2993 4951 1925 3435 3 6 --outpath='out/' SampleImages/targets_1.ARW SampleImages/targets_2.ARW SampleImages/white_1.ARW SampleImages/white_2.ARW SampleImages/dark_1.ARW SampleImages/dark_2.ARW SampleImages/art_A_1.ARW SampleImages/art_A_2.ARW
test_multi:
	mkdir -p out
	dist/BeyondRGB --outpath='out/' 2993 4951 1925 3435 3 6 SampleImages/targets_1.ARW SampleImages/targets_2.ARW SampleImages/white_1.ARW SampleImages/white_2.ARW SampleImages/dark_1.ARW SampleImages/dark_2.ARW SampleImages/art_A_1.ARW SampleImages/art_A_2.ARW SampleImages/art_B_2.ARW SampleImages/art_B_2.ARW  SampleImages/art_C_2.ARW SampleImages/art_C_2.ARW
