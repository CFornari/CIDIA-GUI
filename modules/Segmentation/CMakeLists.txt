cmake_minimum_required(VERSION 3.17)

project(Segmentation LANGUAGES CXX)

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
		"${MITK_SUPERBUILD_DIR}/MITK-build/"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"/home/pop_os/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)


set(cpp_files
#		QmitkAutocropAction.cpp
#		QmitkConvertMaskToLabelAction.cpp
#		QmitkConvertSurfaceToLabelAction.cpp
#		QmitkConvertToMultiLabelSegmentationAction.cpp
#		QmitkCreateMultiLabelPresetAction.cpp
#		QmitkCreateMultiLabelSegmentationAction.cpp
#		QmitkLoadMultiLabelPresetAction.cpp
		QmitkMultiLabelSegmentationView.cpp
#		QmitkSingleNodeSelectionWidget.cpp
#		QmitkNodeSelectionButton.cpp
#		QmitkNodeSelectionDialog.cpp
#		QmitkNodeSelectionPreferenceHelper.cpp
#		QmitkThresholdAction.cpp
		)

set(header_files
#		QmitkAutocropAction.h
#		QmitkConvertMaskToLabelAction.h
#		QmitkConvertSurfaceToLabelAction.h
#		QmitkConvertToMultiLabelSegmentationAction.h
#		QmitkCreateMultiLabelPresetAction.h
#		QmitkCreateMultiLabelSegmentationAction.h
#		QmitkLoadMultiLabelPresetAction.h
		QmitkMultiLabelSegmentationView.h
#		QmitkSingleNodeSelectionWidget.h
#		QmitkNodeSelectionButton.h
#		QmitkNodeSelectionDialog.h
#		QmitkNodeSelectionPreferenceHelper.h
#		QmitkThresholdAction.h
		)

set(ui_files
		QmitkMultiLabelSegmentationControls.ui
#		QmitkSingleNodeSelectionWidget.ui
#		QmitkNodeSelectionDialog.ui
		)

set(qrc_files
		segmentation.qrc
		)

add_library(${PROJECT_NAME} STATIC ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_14)

# Required on Unix OS family to be able to be linked into shared libraries.
#set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)


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

target_include_directories(${PROJECT_NAME} PUBLIC ${AppDataManager_PATH})

target_link_libraries(${PROJECT_NAME} PRIVATE
		MitkCore
		MitkQtWidgetsExt
		MitkSegmentation
		MitkSegmentationUI
		CTKWidgets
		Qt5::Widgets
		AppDataManager
		)

