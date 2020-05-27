cmake_minimum_required(VERSION 3.5)

project(plugins LANGUAGES CXX)

add_subdirectory(thumbnailListView/)

#set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR})

#add_library(plug SHARED
#	GenericPlugin.cpp
#	GenericPlugin.h
#	plug.json
#)

#target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Gui)

#target_compile_definitions(${PROJECT_NAME} PRIVATE PLUG_LIBRARY)

#target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})
