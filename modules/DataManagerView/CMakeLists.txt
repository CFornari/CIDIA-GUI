cmake_minimum_required(VERSION 3.17)

project(DataManagerView LANGUAGES CXX)

# Qt compilations system variables.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON) # enable linker to qt headers
set (CMAKE_AUTOUIC ON) #
set (CMAKE_AUTORCC ON) # enable link to qrc

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(MITK_SUPERBUILD_DIR "/usr/mitk-build")
#set(MITK_SUPERBUILD_DIR "/media/storage/carlo/Documents/MITK/mitk-superbuild-debug")
set(QT_PATH "/home/pop_os")

set(CMAKE_PREFIX_PATH
		"${MITK_SUPERBUILD_DIR}/MITK-build/"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"${QT_PATH}/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)


set(cpp_files
		DataManagerView.cpp
		QmitkDataManagerItemDelegate.cpp
		)

set(header_files
		DataManagerView.h
		QmitkDataManagerItemDelegate.h
		)

set(ui_files
		DataManagerView.ui
		)

add_library(${PROJECT_NAME} STATIC ${cpp_files} ${header_files} ${ui_files})

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_14)

set(AppDataManager_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../AppDataManager/")

###
# Checks whether this project is the main one or a subdirectory.
###
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
		# I am top-level project.
		add_subdirectory(${AppDataManager_PATH} build/)
else()
		# I am called from other project with add_subdirectory().

		# Required on Unix OS family to be able to be linked into shared libraries.
		set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
###

target_link_libraries(${PROJECT_NAME} PUBLIC
		MitkCore
		MitkQtWidgetsExt
		Qt5::Widgets
		AppDataManager
		)

target_include_directories(${PROJECT_NAME} PUBLIC ${AppDataManager_PATH})

