# QUASARTS Orbit Engine
### University of Leeds
#### COMP5530M Group Project

# Documentation #
## [Script System](./project/QUASARTS/Engine/src/Scripts/README.md) 
## [Event System](./project/QUASARTS/Engine/src/Event/README.md)

# Installation #
## Windows
## Linux
Open a terminal and cd into the project/QUASARTS directory.

Be sure to add permission to execute the premake5 program:
(sudo) chmod +x ./premake5

### To compile:
./premake5 gmake2
make

### To run:
./bin/Debug-linux-x86_64/Editor/Editor

### The libraries required by the Linux version of the project are:

g++
libc6-dev

libxi-dev
libxcursor-dev
libxrandr-dev
libxinerama-dev

libsndfile1-dev
libopus-dev
libopenal-dev

### This library *may* be required:

libx11-dev

### These libraries should be installed with libsndfile1-dev but they are listed here just in case:

libflac-dev
libogg-dev
libvorbis-dev

### You can install all of these libraries with:

sudo apt install g++ libc6-dev libxi-dev libxcursor-dev libxrandr-dev libxinerama-dev libsndfile1-dev libopus-dev libopenal-dev
