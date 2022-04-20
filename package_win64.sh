#!/bin/bash
echo "Locating Windows VapourSynth install..."
VAPOURSYNTH_DLL_DIR=$(reg query "HKCU\Software\VapourSynth" | grep VapourSynthDLL | sed "s/    VapourSynthDLL    REG_SZ    //g" | tr "\\" "/")
if [[ "$VAPOURSYNTH_DLL_DIR" == *"vapoursynth.dll" ]]
then
    echo "Windows VapourSynth install located"
else
    VAPOURSYNTH_INSTALL_DIR=$(reg query "HKLM\SOFTWARE\VapourSynth" | grep VapourSynthDLL | sed "s/    VapourSynthDLL    REG_SZ    //g" | tr "\\" "/")
    if [[ "$VAPOURSYNTH_DLL_DIR" == *"vapoursynth.dll" ]]
    then
        echo "Windows VapourSynth install located"
    else
        echo "WARNING: Could not locate Windows VapourSynth install, libvapoursynth.dll and libvapoursynth-script-0.dll will be missing from the package"
    fi
fi
WOBBLY_WIN64_ARCHIVE_NAME=wobbly-win64-$(git rev-parse --short=5 HEAD).7z
echo "Copying required DLLs and folders..."
mkdir platforms
mkdir styles
cp /mingw64/bin/Qt5Core.dll .
cp /mingw64/bin/Qt5Gui.dll .
cp /mingw64/bin/Qt5Widgets.dll .
cp /mingw64/bin/libbrotlicommon.dll .
cp /mingw64/bin/libbrotlidec.dll .
cp /mingw64/bin/libbz2-1.dll .
cp /mingw64/bin/libdouble-conversion.dll .
cp /mingw64/bin/libffi-7.dll .
cp /mingw64/bin/libfreetype-6.dll .
cp /mingw64/bin/libglib-2.0-0.dll .
cp /mingw64/bin/libgraphite2.dll .
cp /mingw64/bin/libharfbuzz-0.dll .
cp /mingw64/bin/libiconv-2.dll .
cp /mingw64/bin/libicudt69.dll .
cp /mingw64/bin/libicuin69.dll .
cp /mingw64/bin/libicuuc69.dll .
cp /mingw64/bin/libintl-8.dll .
cp /mingw64/bin/libmd4c.dll .
cp /mingw64/bin/libpcre-1.dll .
cp /mingw64/bin/libpcre2-16-0.dll .
cp /mingw64/bin/libpng16-16.dll .
cp $VAPOURSYNTH_DLL_DIR libvapoursynth.dll
cp $(echo $VAPOURSYNTH_DLL_DIR | sed "s/vapoursynth.dll//g")/vsscript.dll libvapoursynth-script-0.dll
cp /mingw64/bin/libwinpthread-1.dll .
cp /mingw64/bin/libzimg-2.dll .
cp /mingw64/bin/libzstd.dll .
cp /mingw64/bin/zlib1.dll .
cp /mingw64/bin/libgcc_s_seh-1.dll .
cp /mingw64/bin/libstdc++-6.dll .
cp /mingw64/share/qt5/plugins/platforms/qwindows.dll platforms/
cp /mingw64/share/qt5/plugins/styles/qwindowsvistastyle.dll styles/
echo "Creating 7z archive..."
7za a $WOBBLY_WIN64_ARCHIVE_NAME *.dll *.exe platforms/ styles/
echo "Packaging done: $WOBBLY_WIN64_ARCHIVE_NAME"