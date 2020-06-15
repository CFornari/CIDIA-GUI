cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(covid-gui-desktop LANGUAGES CXX)

#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Qt compilations system variables.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON) # enable linker to qt headers
set (CMAKE_AUTOUIC ON) #
set (CMAKE_AUTORCC ON) # enable link to qrc

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

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
find_package(Qt5 COMPONENTS Widgets Charts Svg REQUIRED)

add_subdirectory(widgets/SpaceXStyle) # a static library
add_subdirectory(widgets/VolumeVisualizationView) # a static library
#add_subdirectory(widgets/ImageNavigator) # a static library

set(cpp_files
		main.cpp
		MainWindow.cpp
		# Database / file managment
		Utils.cpp
		# Interface
		Concept1.cpp
		Concept2.cpp
		Concept3.cpp
		)

set(header_files
		MainWindow.h
		# Database / file managment
		Utils.h
		# Interface
		Concept1.h
		Concept2.h
		Concept3.h
		)

set(ui_files
		MainWindow.ui
		Concept1.ui
		Concept2.ui
		Concept3.ui)

set(qrc_files
		resource.qrc
		)

set(files ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

add_executable(${PROJECT_NAME} ${files})

target_link_libraries(${PROJECT_NAME} PUBLIC
	MitkCore
	MitkQtWidgetsExt
	Qt5::Widgets
	Qt5::Charts
	Qt5::Svg
	SpaceXStyle
	VolumeVisualizationView
#	thumbnailListWidget
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC
		"${CMAKE_SOURCE_DIR}/widgets/SpaceXStyle"
		"${CMAKE_SOURCE_DIR}/widgets/VolumeVisualizationView"
		)
