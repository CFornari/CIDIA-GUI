cmake_minimum_required(VERSION 3.17)

project(ImageNavigator LANGUAGES CXX)

set(MITK_SUPERBUILD_DIR "/usr/mitk-build")

set(CMAKE_PREFIX_PATH
		"${CMAKE_PREFIX_PATH}"
		"${MITK_SUPERBUILD_DIR}/MITK-build/"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"/home/pop_os/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(cpp_files
		QmitkImageNavigatorView.cpp
		)

set(header_files
		QmitkImageNavigatorView.h
		)

set(ui_files
		QmitkImageNavigatorViewControls.ui
		)

set(qrc_files
		imagenavigator.qrc
		)

add_library(${PROJECT_NAME} STATIC ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

# Required on Unix OS family to be able to be linked into shared libraries.
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME} PRIVATE
		MitkCore
		MitkQtWidgetsExt
		Qt5::Widgets
		)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
