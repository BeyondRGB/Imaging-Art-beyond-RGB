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

################################### Building ###################################
################## Unix and Windows use different delimiters ###################
################################################################################ 
ifeq ($(OS),Windows_NT)  # Windows_NT, XP, 2000, 7, 8, 10, 11...
	DELIMITER = ;
else
	DELIMITER = :
endif
BUILD_ARGS = -p src --distpath $(OUT_DIR) -y --clean -F --add-data $(DATA_DIR)$(DELIMITER)$(DATA_DIR)

make:
#ifeq ($(OS),Windows_NT)  # Windows_NT, XP, 2000, 7, 8, 10, 11...
#	python.exe -m PyInstaller $(BUILD_ARGS) src/BeyondRGB.py
#else
	pyinstaller $(BUILD_ARGS) src/BeyondRGB.py
#endif

clean:
ifeq ($(OS),Windows_NT)  # Windows_NT, XP, 2000, 7, 8, 10, 11...
	del .\BeyondRGB.spec
	rmdir /S /Q .\dist
	rmdir /S /Q .\build
	rmdir /S /Q .\out
else
	rm BeyondRGB.spec dist build out -rf

venv-init:
	( \
	 	python3 -m venv ./venv; \
	 	. venv/bin/activate; \
		pip install -r ../requirements.txt; \
	)

venv-clean:
	rm -rf venv
	find -iname "*.pyc" -delete



################################### Testing ###################################
###### FOR TEST CASES UNZIP GITHUB EXAMPLE IMAGES IN TOP LEVEL DIRECTORY ######
###############################################################################
test_single:
	mkdir -p out
	dist/BeyondRGB cli 2993 4951 1925 3435 3 6 --outpath='out/' -t NGT SampleImages/targets_1.ARW SampleImages/targets_2.ARW SampleImages/white_1.ARW SampleImages/white_2.ARW SampleImages/dark_1.ARW SampleImages/dark_2.ARW SampleImages/art_A_1.ARW SampleImages/art_A_2.ARW
test_multi:
	mkdir -p out
	dist/BeyondRGB cli --outpath='out/' -t NGT 2993 4951 1925 3435 3 6 SampleImages/targets_1.ARW SampleImages/targets_2.ARW SampleImages/white_1.ARW SampleImages/white_2.ARW SampleImages/dark_1.ARW SampleImages/dark_2.ARW SampleImages/art_A_1.ARW SampleImages/art_A_2.ARW SampleImages/art_B_2.ARW SampleImages/art_B_2.ARW  SampleImages/art_C_2.ARW SampleImages/art_C_2.ARW
