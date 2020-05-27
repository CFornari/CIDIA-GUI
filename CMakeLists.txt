cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Qt compilations system variables.
set (CMAKE_AUTOMOC ON) # enable linker to qt headers
set (CMAKE_AUTOUIC ON) #
set (CMAKE_AUTORCC ON) # enable link to qrc

project(covid-gui-desktop LANGUAGES CXX)

set(MITK_SUPERBUILD_DIR "/media/storage/carlo/Documents/MITK/mitk-superbuild-debug")

set(CMAKE_PREFIX_PATH
		"${CMAKE_PREFIX_PATH}"
		"${CMAKE_SOURCE_DIR}/lib/GalleryCore/"
		"${CMAKE_SOURCE_DIR}/lib/FramelessWindow/"
		"${MITK_SUPERBUILD_DIR}/MITK-build/"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"/home/pop_os/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(GalleryCore REQUIRED)
find_package(FramelessWindow REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/lib/QtFramelessWindowDarkStyle/framelesswindow/")

add_subdirectory(plugins/)

set(cpp_files
		main.cpp
		# GUI related
		MainWindow.cpp
		MainDisplay.cpp
		SliceAnimationControls.cpp
		# Database / file managment
		Utils.cpp
		)

set(header_files
		# GUI related
		MainWindow.h
		MainDisplay.h
		SliceAnimationControls.h
		# Database / file managment
		Utils.h
		)

set(ui_files
		MainWindow.ui)

set(qrc_files
#		themes/qdarkstyle/style.qrc
#		themes/breeze/breeze.qrc
#		themes/framelessdark/darkstyle.qrc
		)

set(files ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

add_executable(${PROJECT_NAME} ${files})

target_link_libraries(${PROJECT_NAME}
	MitkCore
	MitkQtWidgetsExt
	Qt5::Widgets
	GalleryCore
	FramelessWindow
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
