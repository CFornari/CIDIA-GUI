# Step 6

Interactively region-grower. 

In this step the program is enhanced by the possibility to start a region-grower at interactively added points. We will see how MITK images can be accessed as ITK images. We now load the image file Pic3D.nrrd only since the surface will be the result of the region-growing.

Add points in the image by pressing SHIFT+left mouse key, then adjust the thresholds and press 'Start region growing'.
Before build change QT_DIR and MITK_SUPERBUILD_DIR at CMakeLists.txt to match the paths of your system.

Build steps:

```bash
cmake -B build/ -S .
cd build
make
```

Execute:

```bash
./step3 [file1-path] [file2-path]
```
