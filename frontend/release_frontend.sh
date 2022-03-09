#!/bin/sh

if [ -e ../backend/build/Release/app ]
then
	echo Found ../backend/build/Release/app

	# Make sure the backend/lib folder exists
	if [ -d ./lib ]
	then
		echo Found ./lib
	else
		mkdir -p ./lib
	fi

	# Make sure the backend/res folder exists
	if [ -d ./res ]
	then
		echo Found ./res
	else
		mkdir -p ./res
	fi

	# Copy executable
	cp -v ../backend/build/Release/app ./lib

	# Copy all backend resource files
	cp -rv ../backend/res/* ./res

	npm run make

else
	echo Please build the backend release.
fi
