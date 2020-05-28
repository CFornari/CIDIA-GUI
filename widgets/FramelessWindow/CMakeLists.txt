cmake_minimum_required(VERSION 3.17)

project(FramelessWindow LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH
		"${CMAKE_PREFIX_PATH}"
		"/home/pop_os/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(cpp_files
		framelesswindow.cpp
		windowdragger.cpp
		DarkStyle.cpp
		)

set(header_files
		framelesswindow.h
		windowdragger.h
		DarkStyle.h
		)

set(ui_files
		framelesswindow.ui
		)

set(qrc_files
		framelesswindow.qrc
		darkstyle.qrc
		)

add_library(${PROJECT_NAME} STATIC ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_14)

# Required on Unix OS family to be able to be linked into shared libraries.
#set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME} PRIVATE	Qt5::Widgets)
