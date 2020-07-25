cmake_minimum_required(VERSION 3.17)

project(DarkStyle LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT DEFINED QT_PATH)
		set(QT_PATH "/home/pop_os/Qt/5.12.8")
endif()

set(CMAKE_PREFIX_PATH
		"${QT_PATH}/gcc_64/lib/cmake/Qt5/"
		)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(cpp_files
		DarkStyle.cpp
		)

set(header_files
		DarkStyle.h
		)

set(qrc_files
		darkstyle.qrc
		)

add_library(${PROJECT_NAME} STATIC ${cpp_files} ${header_files} ${qrc_files})

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_14)

# Required on Unix OS family to be able to be linked into shared libraries.
#set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME} PRIVATE	Qt5::Widgets)
