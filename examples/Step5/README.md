#Step 5
Interactively add points. 

A state machine describes interaction pattern with different states (states beeing something like "a point is selected") and transitions to these states (e.g. "select a point"). These transitions are associated with actions. In this way it is possible to model complex interaction schemes. By what these transitions and actions are triggered is described in a configuration file. It maps user events to identifiers that are used in the state machine patterns. In this way the user interaction can be changed by simply loading a different configuration file for a state machine, and the user may add points now with a right click instead of left click + SHIFT, as in our case.

In order to add a point the shift key has to be pressed while left clicking in a render window. You can also move points or remove them (left click while pressing ALT).

Before build change QT_DIR and MITK_SUPERBUILD_DIR at CMakeLists.txt to match the paths of your system.

Build steps:
cmake -B build/ -S .
cd build
make

Execute:
./step5 [file1-path] [file2-path]
