cmake_minimum_required(VERSION 3.17)

project(VolumeVisualizationView LANGUAGES CXX)

# Qt compilations system variables.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON) # enable linker to qt headers
set (CMAKE_AUTOUIC ON) #
set (CMAKE_AUTORCC ON) # enable link to qrc

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

if(NOT DEFINED MITK_SUPERBUILD_DIR)
		set(MITK_SUPERBUILD_DIR "/usr/mitk-build")
endif()

if(NOT DEFINED QT_PATH)
		set(QT_PATH "/home/pop_os/Qt/5.12.8")
endif()

set(CMAKE_PREFIX_PATH
		"${MITK_SUPERBUILD_DIR}/MITK-build/"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"${QT_PATH}/gcc_64/lib/cmake/Qt5/"
		)


find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)


set(cpp_files
		VolumeVisualizationView.cpp
		)

set(header_files
		VolumeVisualizationView.h
		)

set(ui_files
		VolumeVisualizationView.ui
		)

set(qrc_files
		)

add_library(${PROJECT_NAME} STATIC ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_14)

# Required on Unix OS family to be able to be linked into shared libraries.
#set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME} PRIVATE
		MitkCore
		MitkQtWidgetsExt
		Qt5::Widgets
		)
