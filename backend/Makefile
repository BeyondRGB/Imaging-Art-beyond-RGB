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
DATA_DIR = src/data

BUILD_ARGS_BASE = --specpath $(OUT_DIR) --distpath $(OUT_DIR) -y --clean -F

BUILD_ARGS_UNIX = $(BUILD_ARGS_BASE) --add-data *:$(DATA_DIR)
BUILD_ARGS_WIN = $(BUILD_ARGS_BASE) --add-data *;$(DATA_DIR)

make: unix

unix:
	pyinstaller $(BUILD_ARGS_UNIX) src/main.py

win:
	pyinstaller $(BUILD_ARGS_WIN) src/main.py
