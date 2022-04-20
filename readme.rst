Description
===========

There are two executables, Wibbly and Wobbly.

Wibbly gathers metrics and creates project files that Wobbly can open. (Wobbly can also open video files directly.)

Wobbly can be used to filter a video per scene, and/or to improve upon VFM and VDecimate's decisions.


Compilation (\*nix)
===================

Requirements:
    - A C++11 compiler.

    - Qt 5.4 or newer (just qtbase).

    - VapourSynth r32 or newer.

The usual steps work::

    ./autogen.sh
    ./configure
    make


Compilation (Windows) - gcc
===========================

Compilation on Windows is possible by using gcc via MSYS2 64 bit. I have also included a packaging script to create a 7z archive with all the needed folders and DLLs.

Requirements:
    - MSYS2 x86_64
    - VapourSynth R57 x86_64

MSYS2 Packages:
    - autoconf
    - automake
    - git
    - p7zip
    - libtool
    - make
    - pkg-config
    - mingw-w64-x86_64-vapoursynth
    - mingw-w64-x86_64-qt5-base
    - mingw-w64-x86_64-gcc

Follow these steps to setup your build environment on Windows:

1. Install MSYS2 64 bit
2. Start the MSYS2 shell via the provided "MSYS2 MinGW x64" shortcut

Update the pacman package repositories by running this command::
    
    pacman -Syy

Then, execute the following command to install all build and packaging dependencies::
    
    pacman -S autoconf automake git p7zip libtool make pkg-config mingw-w64-x86_64-vapoursynth mingw-w64-x86_64-qt5-base mingw-w64-x86_64-gcc

You can now follow the usual steps to clone the repo, compile, and use the script I provided for packaging at the end, if you wish::

    git clone https://github.com/Wyse-/Wobbly.git
    cd Wobbly/
    ./autogen.sh
    ./configure
    make
    ./package_win64.sh

This will create a 7z archive of the built executables with all DLLs and required folders.

Compilation (Windows) - Clang
=============================

Compiling via gcc is generally recommended, but if any issues arise you can alternatively compile with Clang. Clang compilation instructions for Windows are available in the "doc" folder of this repository:
https://github.com/Wyse-/Wobbly/blob/master/doc/windows_clang_compilation.rst.

Keep in mind that when building with Clang, the produced executables will not be compatible with the LibP2P.dll provided by vsrepo, therefore you will have to either build your own with Clang or use a precompiled version:
https://github.com/Wyse-/Wobbly/releases/download/v6.0.1-beta/LibP2P.dll.

License
=======

The code itself is available under the ISC license.
