cmake_minimum_required(VERSION 3.17)

project(ThumbnailListView LANGUAGES CXX)

# Qt compilations system variables.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON) # enable linker to qt headers
set (CMAKE_AUTOUIC ON) #
set (CMAKE_AUTORCC ON) # enable link to qrc

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(QT_PATH "/home/pop_os")

set(CMAKE_PREFIX_PATH
		"${CMAKE_PREFIX_PATH}"
		"${QT_PATH}/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../lib/GalleryCore/"
		)


find_package(Qt5 COMPONENTS Widgets Charts Concurrent Sql REQUIRED)
find_package(GalleryCore REQUIRED)

set(cpp_files
		PictureDelegate.cpp
		ThumbnailListView.cpp
		ThumbnailProxyModel.cpp
		ThumbnailListLoader.cpp
		)

set(header_files
		PictureDelegate.h
		ThumbnailListView.h
		ThumbnailProxyModel.h
		ThumbnailListLoader.h
		)

add_library(${PROJECT_NAME} STATIC ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_14)

# Required on Unix OS family to be able to be linked into shared libraries.
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME} PUBLIC
		Qt5::Widgets
		Qt5::Concurrent
		Qt5::Sql
		GalleryCore
		)

target_include_directories(${PROJECT_NAME} PUBLIC
		"${CMAKE_SOURCE_DIR}/lib/GalleryCore/include/"
		)
