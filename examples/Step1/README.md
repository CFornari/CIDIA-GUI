#Step 1
Shows how to use a renderwindow to show one slice from the data set.

Before build change QT_DIR and MITK_SUPERBUILD_DIR at CMakeLists.txt to match the paths of your system.

Build steps:
cmake -B build/ -S .
cd build
make

Execute:
./step1 [file-path]
