# QUASARTS Orbit Engine
### University of Leeds
#### COMP5530M Group Project

# Documentation #
## [Script System](./project/QUASARTS/Engine/src/Scripts/README.md) 
## [Event System](./project/QUASARTS/Engine/src/Event/README.md)

# Installation #
## Windows

Open the project folder in Visual Studio.

In Visual Studio, open the Terminal/PowerShell view and cd to the QUASARTS/project/QUASARTS directory.

### To compile:
In the Terminal/PowerShell prompt:

`./premake5 vs*`

where * is your Visual Studio version, e.g., `./premake5 vs2022`.

Build the resulting QUASARTS solution.

### To run:
Open the QUASARTS solution in Visual Studio.

Set the Editor project as the startup project and run.

## Linux

Open a terminal and cd into the project/QUASARTS directory.

Be sure to add permission to execute the premake5 program:\
(sudo) chmod +x ./premake5

### To compile:
./premake5 gmake2\
make

### To run:
./bin/Debug-linux-x86_64/Editor/Editor

### Version:

Tested on Linux Mint 20.1 and 20.3.

### The libraries required by the Linux version of the project are:

g++\
libc6-dev

libxi-dev\
libxcursor-dev\
libxrandr-dev\
libxinerama-dev

libsndfile1-dev\
libopus-dev\
libopenal-dev

### You can install all of these libraries with:

sudo apt install g++ libc6-dev libxi-dev libxcursor-dev libxrandr-dev libxinerama-dev libsndfile1-dev libopus-dev libopenal-dev

### This library *may* be required:

libx11-dev

### These libraries should be installed with libsndfile1-dev but they are listed here just in case:

libflac-dev\
libogg-dev\
libvorbis-dev
