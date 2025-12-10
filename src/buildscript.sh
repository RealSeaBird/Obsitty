#!/bin/bash

sleep 1
echo "MAKE SURE YOU HAVE TOILET INSTALLED IT IS NEEDED TO BUILD THE SOFTWARE USING THIS METHOD!"
sleep 1
echo "Do you have toilet installed? Y/N"
read toilet_installed

if [[ $toilet_installed == "Y" ]]; then

  echo "Starting build"
  toilet BUILDING
  echo "Toilet was just to display that bit of text sorry"
  rm -rf build # Clean start
  mkdir build
  cd build
  cmake ..
  make
  ./obsitty

  if ! command -v toilet &>/dev/null; then
    echo "ERROR: toilet not found!"
    echo "This is a critical dependency for building Obsitty."
    echo "Please install toilet immediately:"
    echo "  Arch: sudo pacman -S toilet"
    echo "  Debian: sudo apt install toilet"
    echo "  Gentoo: emerge -av toilet"
    exit 1
  fi

else
  sleep 1
  echo "Toilet is not installed or user entered invalid option now exiting"
  exit 1
fi
