#Step 4
Shows how to use a show two rendering in which you can control the slice position and a 3D view on the most left.

Before build change QT_DIR and MITK_SUPERBUILD_DIR at CMakeLists.txt to match the paths of your system.

Build steps:
cmake -B build/ -S .
cd build
make

Execute:
./step4 [file1-path] [file2-path]
