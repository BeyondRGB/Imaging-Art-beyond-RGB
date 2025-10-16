#!/bin/sh

if [ -e ../backend/build/Release/beyond-rgb-backend ]
then

	if [ -d "./lib/" ]; then rm -rf "./lib/"; fi
	mkdir "./lib/"

	if [ -d "./res/" ]; then rm -rf "./res/"; fi
	mkdir "./res/"

	# Copy executable
	cp -v ../backend/build/Release/beyond-rgb-backend ./lib

	# Copy all backend resource files
	cp -rv ../backend/res/* ./res

	npm run make

else
	echo Please build the backend release.
fi
