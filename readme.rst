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


Compilation (Windows)
=====================

Compilation on Windows is possible by using clang via MSYS2 64 bit. I have also included a packaging script to create a 7z archive with all the needed folders and DLLs.

Requirements:
    - MSYS2 x86_64

MSYS2 Packages:
    - autoconf-wrapper
    - automake-wrapper
    - git
    - p7zip
    - libtool
    - make
    - pkg-config
    - mingw-w64-clang-x86_64-vapoursynth
    - mingw-w64-clang-x86_64-qt5-base
    - mingw-w64-clang-x86_64-clang

Follow these steps to setup your build environment on Windows:

1. Install MSYS2 64 bit
2. Start the MSYS2 shell via the provided "MSYS2 MinGW Clang x64" shortcut

Then, execute the following command to install all build and packaging dependencies::
    
    pacman -S autoconf-wrapper automake-wrapper git p7zip libtool make pkg-config mingw-w64-clang-x86_64-vapoursynth mingw-w64-clang-x86_64-qt5-base mingw-w64-clang-x86_64-clang

You can now follow the usual steps to clone the repo, compile, and use the script I provided for packaging at the end, if you wish::

    git clone https://github.com/Wyse-/Wobbly.git
    cd Wobbly/
    ./autogen.sh
    ./configure
    make
    ./package_win64.sh

This will create a 7z archive of the built executables with all DLLs and required folders.


LibP2P Compilation (Windows)
============================
As this build does not work with the LibP2P version which is shipped with vsrepo, you may need to compile your own, which you can accomplish in the same MSYS2 build environment used to compile wobbly.
To compile LibP2P, you will need to install a few extra MSYS2 packages.

MSYS2 Packages:
    - mingw-w64-clang-x86_64-meson
    - mingw-w64-clang-x86_64-cmake

Install the packages mentioned above with the following command::

    pacman -S mingw-w64-clang-x86_64-meson mingw-w64-clang-x86_64-cmake

Once that is done, you can clone the LibP2P repo and proceed with the usual meson based build process::
    
    git clone https://github.com/DJATOM/LibP2P-Vapoursynth.git
    cd LibP2P-Vapoursynth/
    meson build
    cd build/
    ninja

This will produce a clang compiled LibP2P.dll, which you can use with this specific wobbly build. Note that this clang compiled LibP2P DLL may not work with other wobbly versions.


License
=======

The code itself is available under the ISC license.
