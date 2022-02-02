if [ -e ../backend/build/Release/*.exe ]
then
	echo Found ../backend/build/Release/*.exe
	if [ -d ./backend/ ]
	then 
		echo Found ./backend/
	else
		mkdir ./backend/
	fi
	if [ -d ./backend/lib/ ]
	then
		echo Found ./backend/lib/
	else
		mkdir ./backend/lib/
	fi
	if [ -d ./backend/res/ ]
	then
		echo Found ./backend/res/
	else
		mkdir ./backend/res/
	fi
	# Copy .exe and libraries 
	cp ../backend/build/Release/* ./backend/lib
	# Copy all backend resource files
	cp -r ../backend/res ./backend

	npm run make

else
	echo Backend Release build does not exist. Run Imaging-Art-beyond-RGB/backend/linux_release.sh and try again
fi

$SHELL