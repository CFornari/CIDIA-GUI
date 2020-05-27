cmake_minimum_required(VERSION 3.5)

project(thumbnailListView LANGUAGES CXX)

set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR})

set(cpp_files
		PictureDelegate.cpp
		ThumbnailListView.cpp
		ThumbnailListWidget.cpp
		ThumbnailProxyModel.cpp
		)

set(hpp_files
		PictureDelegate.h
		ThumbnailListView.h
		ThumbnailListWidget.h
		ThumbnailProxyModel.h
		)

add_library(${PROJECT_NAME} SHARED ${cpp_files} ${hpp_files} plug.json)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Gui)

target_compile_definitions(${PROJECT_NAME} PRIVATE PLUG_LIBRARY)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})
