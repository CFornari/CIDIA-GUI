cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

set (CMAKE_AUTOMOC ON)

project(Step7)

set(MITK_SUPERBUILD_DIR "/usr/mitk-build")
set(QT_DIR "/home/pop_os")

set(CMAKE_PREFIX_PATH
		"${MITK_SUPERBUILD_DIR}/MITK-build"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"${QT_DIR}/Qt/5.12.8/gcc_64/lib/cmake/Qt5"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

set(cpp_files
		Step7.cpp
		Step7main.cpp
		../Step6/Step6RegionGrowing1.cpp
		../Step6/Step6RegionGrowing2.cpp
		../Step6/Step6RegionGrowing3.cpp
		../Step6/Step6.cpp
		)

set(header_files
		../Step6/Step6.h
		Step7.h
		)

add_executable(${PROJECT_NAME} ${cpp_files} ${header_files})
target_link_libraries(${PROJECT_NAME}
		MitkCore
		MitkQtWidgetsExt)
