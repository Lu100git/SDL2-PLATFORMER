#!/bin/bash

if [[ `whoami` != root ]]; then
	echo "you must be root to execute this program, TRUST ME BRO!"
	echo "if you have sudo installed you can use sudo su"
	echo "if you don't trust this script is ok"
	echo "you can always compile the program yourself"
	echo "dependencies needed for compilation: [make, cmake, c++, and SDL developer libraries]"
	echo "dependencies needed to execute the binary file: [SDL2, SDL2_image and SDL2_mixer]"
	exit
fi

clear

echo "	.: Platformer Compiler script (Debian version) :."
echo "in order to properly compile this program"
echo "c++ and SDL need to be installed in this machine"
echo "this script will temporarily install: [make, cmake, g++, and SDL developer libraries]"
echo -n "do you wish to proceed? [Y/n]: "
read first_choice

if [[ "$first_choice" == "y" || "$first_choice" == "Y" ]]; then
	g++ --version 2> /dev/null
	if [[ $? != 0 ]]; then
		echo " # installing C++ compiler..."
		sudo apt install g++ -y
	else
		echo "g++ is already installed!"
	fi

	if ! dpkg -l | grep -q libsdl2-dev; then
		echo "# installing SDL2..."
		sudo apt install libsdl2-dev -y
		echo "# installing SDL2 external libraries..."
		sudo apt install libsdl2-image-dev -y
		sudo apt install libsdl2-mixer-dev -y
	else
		echo "SDL2 is already installed!"
		echo "dont forget to install the external libraries"
		echo -n "do you wish to install the external libraries? [Y/n]: "
		read libraries_choice
		if [[ "$libraries_choice" == "Y" || "$libraries_choice" == "y" ]]; then
			echo "# installing SDL2 external libraries..."
			sudo apt install libsdl2-image-dev -y
			sudo apt install libsdl2-mixer-dev -y
		else
			echo "awesome!, resuming installation"
		fi
	fi

	make --version 2> /dev/null
	if [[ $? != 0 ]]; then
		echo "# installing make..."
		sudo apt install make -y
	else
		echo "Make is already installed!"
	fi

	cmake --version 2> /dev/null
	if [[ $? != 0 ]]; then
		echo "# installing cmake..."
		sudo apt install cmake -y
	else
		echo "cmake is already installed!"
	fi

	echo "# dependencies have been installed"
	echo "# compiling the project..."
	sleep 3	

	cmake ../SDL2-PLATFORMER
	make
	chmod 777 main

	echo "# deleting developer libraries"
	sleep 3
	sudo apt remove --purge libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev -y

	echo "	# compiler tools will be removed now #"
	echo "	g++, make and cmake"
	echo "	do you wish to remove them? [Y/n]: "
	read choice
	if [[ "$choice" == "Y" || "$choice" == "y" ]]; then
		sudo apt remove --purge cmake make g++ -y
	else
		echo "awesome"
	fi

	echo "	# compilation has finished! "
	sleep 1
	echo "in order to execute the binary"
	echo "you'll need the SDL runtime environment packages"
	echo "if you already have them installed"
	echo "you can execute the binary now, otherwise"
	echo "this script can do it for you"
	echo -n "do you wish to install SDL runtime packages? [Y/n]: "
	read package_choice

	if [[ "$package_choice" == "y" || "$package_choice" == "Y" ]]; then
		sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 -y
	else
		echo "no problem"
		echo "don't forget to install: libsdl2-image-2.0-0 libsdl2-mixer-2.0-0"
	fi

else
	echo "no worries, have a good day!"
	echo "don't forget to install SDL2 before compiling with cmake!"
fi

echo " ######## SCRIPT HAS FINISHED! ###########"
echo "now exit from root, and run the binary as a normal user, it should work now"
echo "have a good day"   
