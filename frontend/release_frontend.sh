#!/bin/sh

if [ -e ../backend/build/Release/app ]
then
	echo Found ../backend/build/Release/app

	# Make sure the backend/lib folder exists
	if [ -d ./backend/lib ]
	then
		echo Found ./backend/lib
	else
		mkdir -p ./backend/lib
	fi

	# Make sure the backend/res folder exists
	if [ -d ./backend/res ]
	then
		echo Found ./backend/res
	else
		mkdir -p ./backend/res
	fi

	# Copy executable
	cp -v ../backend/build/Release/app ./backend/lib

	# Copy all backend resource files
	cp -rv ../backend/res/ ./backend/res/

	npm run make

else
	echo Please build the backend release.
fi
