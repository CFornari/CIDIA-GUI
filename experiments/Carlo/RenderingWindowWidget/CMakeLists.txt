cmake_minimum_required(VERSION 3.5)

project(RenderingWindowWidget LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MITK_SUPERBUILD_DIR "/usr/mitk-build")
#set(MITK_SUPERBUILD_DIR "/media/storage/carlo/Documents/MITK/mitk-superbuild-debug")

set(CMAKE_PREFIX_PATH
		"${CMAKE_PREFIX_PATH}"
		"${MITK_SUPERBUILD_DIR}/MITK-build/"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"/home/pop_os/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
		main.cpp
		MainWindow.cpp
		MainWindow.h
		w3D.cpp
		w3D.h
		wMulti.cpp
		wMulti.h

		MainWindow.ui
		)

target_link_libraries(${PROJECT_NAME} PRIVATE
		MitkCore
		MitkQtWidgetsExt
		Qt5::Widgets
		)
