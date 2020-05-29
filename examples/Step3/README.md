#Step 3
Shows how to use a renderwindow to show a 3D view from the data set.

Before build change QT_DIR and MITK_SUPERBUILD_DIR at CMakeLists.txt to match the paths of your system.

Build steps:
cmake -B build/ -S .
cd build
make

Execute:
./step3 [file1-path] [file2-path]
