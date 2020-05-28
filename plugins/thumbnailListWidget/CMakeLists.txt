cmake_minimum_required(VERSION 3.17)

set(PLUGIN_NAME thumbnailListWidget)

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

add_library(${PLUGIN_NAME} SHARED ${cpp_files} ${hpp_files} plug.json)

target_link_libraries(${PLUGIN_NAME} PUBLIC Qt5::Widgets GalleryCore)

target_compile_definitions(${PLUGIN_NAME} PRIVATE PLUG_LIBRARY)

target_include_directories(${PLUGIN_NAME} PUBLIC
		${INCLUDE_DIRECTORIES}
		${CMAKE_CURRENT_SOURCE_DIR}
		)
