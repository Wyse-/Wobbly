WOBBLY_WIN64_ARCHIVE_NAME=wobbly-win64-$(git rev-parse --short=5 HEAD).7z
echo "Copying required DLLs and folders..."
mkdir platforms
mkdir lib
mkdir styles
cp /clang64/bin/Qt5Core.dll .
cp /clang64/bin/Qt5Gui.dll .
cp /clang64/bin/Qt5Widgets.dll .
cp /clang64/bin/libbrotlicommon.dll .
cp /clang64/bin/libbrotlidec.dll .
cp /clang64/bin/libbz2-1.dll .
cp /clang64/bin/libc++.dll .
cp /clang64/bin/libdouble-conversion.dll .
cp /clang64/bin/libffi-7.dll .
cp /clang64/bin/libfreetype-6.dll .
cp /clang64/bin/libglib-2.0-0.dll .
cp /clang64/bin/libgraphite2.dll .
cp /clang64/bin/libharfbuzz-0.dll .
cp /clang64/bin/libiconv-2.dll .
cp /clang64/bin/libicudt69.dll .
cp /clang64/bin/libicuin69.dll .
cp /clang64/bin/libicuuc69.dll .
cp /clang64/bin/libintl-8.dll .
cp /clang64/bin/libmd4c.dll .
cp /clang64/bin/libpcre-1.dll .
cp /clang64/bin/libpcre2-16-0.dll .
cp /clang64/bin/libpng16-16.dll .
cp /clang64/bin/libpython3.9.dll .
cp /clang64/bin/libunwind.dll .
cp /clang64/bin/libvapoursynth-script-0.dll .
cp /clang64/bin/libvapoursynth.dll .
cp /clang64/bin/libwinpthread-1.dll .
cp /clang64/bin/libzimg-2.dll .
cp /clang64/bin/libzstd.dll .
cp /clang64/bin/zlib1.dll .
cp /clang64/share/qt5/plugins/platforms/qwindows.dll platforms/
cp /clang64/share/qt5/plugins/styles/qwindowsvistastyle.dll styles/
cp -r /clang64/lib/python3.9 lib/
echo "Creating 7z archive..."
7za a $WOBBLY_WIN64_ARCHIVE_NAME *.dll *.exe platforms/ styles/ lib/
echo "Packaging done: $WOBBLY_WIN64_ARCHIVE_NAME"